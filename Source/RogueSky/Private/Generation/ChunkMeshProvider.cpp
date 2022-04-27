// Fill out your copyright notice in the Description page of Project Settings.


#include "Generation/ChunkMeshProvider.h"
#include "Generation/GenerationConstants.h"
#include "Generation/Chunk.h"
#include "Runtime/Core/Public/Async/ParallelFor.h"
using namespace GenerationConstants;

void UChunkMeshProvider::Initialize() {
    TArray<FRuntimeMeshLODProperties> lodProperties;
    lodProperties.Init(FRuntimeMeshLODProperties(), 1);
    lodProperties[0].ScreenSize = 0.75f;
    //lodProperties[1].ScreenSize = 0.25f;
    //lodProperties[2].ScreenSize = 0.05f;

    ConfigureLODs({ lodProperties });
    SetupMaterialSlot(0, FName("Chunk Material"), GetChunkMaterial());

    FRuntimeMeshSectionProperties Properties;
    Properties.bWants32BitIndices = false;
    Properties.bUseHighPrecisionTangents = false;
    Properties.bUseHighPrecisionTexCoords = false;
    Properties.bIsVisible = true;
    Properties.bCastsShadow = true;
    Properties.MaterialSlot = 0;
    Properties.UpdateFrequency = ERuntimeMeshUpdateFrequency::Infrequent;

    CreateSection(0, 0, Properties);
    //CreateSection(1, 0, Properties);
    //CreateSection(2, 0, Properties);
}

FBoxSphereBounds UChunkMeshProvider::GetBounds() {
    FVector boxMin = chunk->GetLocation().ToVector() * CHUNK_SIZE;
    FVector boxMax = (chunk->GetLocation() + FIntVector8(1,1,1)).ToVector() * CHUNK_SIZE;
    return FBoxSphereBounds (FBox(boxMin, boxMax));
}

bool UChunkMeshProvider::GetSectionMeshForLOD(int32 LODIndex, int32 SectionID, FRuntimeMeshRenderableMeshData& MeshData) {
    check(SectionID == 0);
    if (chunk->GetState() < ChunkState::HasOctree) {
        return false;
    }

    TMap<FIntVector8, int> indexMap;
    GenerateVerts(indexMap, LODIndex, true, [&](FVector Location, FVector Normal) {
        MeshData.Positions.Add(FVector3f(Location));
        FVector tangent;
        FVector t1 = FVector::CrossProduct(Normal, FVector::ForwardVector);
        FVector t2 = FVector::CrossProduct(Normal, FVector::UpVector);
        FVector t3 = FVector::CrossProduct(Normal, FVector::RightVector);
        if (t1.Size() >= t2.Size() && t1.Size() >= t3.Size())
            tangent = t1;

        if (t2.Size() >= t1.Size() && t2.Size() >= t3.Size())
            tangent = t2;

        if (t3.Size() >= t1.Size() && t3.Size() >= t2.Size())
            tangent = t3;

        MeshData.Tangents.Add(FVector3f(Normal), FVector3f(tangent));
    });
    GenerateTris(indexMap, LODIndex, [&](int32 TriangleIndices[6]) {
        for (int i = 0; i < 6; i++)
            MeshData.Triangles.Add(TriangleIndices[i]);
    });

    MeshData.Colors.SetNum(MeshData.Positions.Num());
    MeshData.TexCoords.SetNum(MeshData.Positions.Num());

    chunk->SetState(ChunkState::HasMesh);
    return true;
}

Chunk* UChunkMeshProvider::GetChunk() {
    FScopeLock(&this->chunkMutex);
    return chunk;
}
void UChunkMeshProvider::SetChunk(Chunk* Chunk) {
    FScopeLock(&this->chunkMutex);
    chunk = Chunk;
}
UMaterialInterface* UChunkMeshProvider::GetChunkMaterial() {
    FScopeLock(&this->chunkMutex);
    return material;
}
void UChunkMeshProvider::SetChunkMaterial(UMaterialInterface* Material) {
    FScopeLock(&this->chunkMutex);
    material = Material;
}

void UChunkMeshProvider::GenerateVerts(TMap<FIntVector8, int>& IndexMap, int LODLevel, bool CalculateNormals, TFunctionRef<void(FVector Location, FVector Normal)> AddFunction) {
    const int lodChunkResolution = FMath::Exp2(MAX_LOD_DEPTH - LODLevel);
    int vertCount = 0;
    for (int x = 0; x < lodChunkResolution; x++)
    for (int y = 0; y < lodChunkResolution; y++)
    for (int z = 0; z < lodChunkResolution; z++) {
        FVector vertexLocation = chunk->GetVertexLocation(x, y, z, LODLevel);
        if (vertexLocation != FVector::ZeroVector) {
            IndexMap.Add(FIntVector8(x, y, z), vertCount);
            vertCount++;

            FVector normal = FVector::ZeroVector;
            if (CalculateNormals)
                normal = chunk->distanceField.GetNormal(x, y, z, LODLevel);

            AddFunction(vertexLocation, normal);
        }
    }
}

void UChunkMeshProvider::GenerateTris(TMap<FIntVector8, int>& IndexMap, int LODLevel, TFunctionRef<void(int32 TriangleIndices[6])> AddFunction) {
    const int lodChunkResolution = FMath::Exp2(MAX_LOD_DEPTH - LODLevel);
    for (int x = 0; x < lodChunkResolution; x++)
    for (int y = 0; y < lodChunkResolution; y++)
    for (int z = 0; z < lodChunkResolution; z++) {
        FIntVector8 location = FIntVector8(x, y, z);

        FFloat16 e1 = chunk->distanceField.GetDistance(location, LODLevel);
        if (e1 == 0.0f)
            e1 = 1.0f;

        for (int e = 0; e < 3; e++) { // Check the edges of every vertex's cell
            FIntVector8 e2Loc = location + triangleEdgeTable[e]; // Get the location of the edges second vertex using the table
            FFloat16 e2 = chunk->distanceField.GetDistance(e2Loc, LODLevel);

            // Ensure that zero distances count as a positive number
            if (e2 == 0)
                e2 = 1;

            // If there's a sign change, create a plane perpidicular to the edge
            if (e1 * e2 <= 0) {
                TArray<int> triPoints;
                for (int t = 0; t < 4; t++) {
                    FIntVector8 triPoint = location + triangulationTable[e][t];

                    // Stop if the tripoint is outside the chunk
                    if (triPoint.X < 0 || triPoint.Y < 0 || triPoint.Z < 0)
                        break;
                    if (triPoint.X >= lodChunkResolution || triPoint.Y >= lodChunkResolution || triPoint.Z >= lodChunkResolution)
                        break;

                    if (!IndexMap.Contains(triPoint))
                        break;
                    int vertex = IndexMap[triPoint];
                    if (vertex == -1)
                        break;

                    triPoints.Add(vertex);
                }
                // Generate the plane from the vertecies given
                if (triPoints.Num() >= 4) {
                    int32 triangleIndices[6];
                    if (e1 > e2) {
                        triangleIndices[0] = triPoints[0];
                        triangleIndices[1] = triPoints[1];
                        triangleIndices[2] = triPoints[2];
                        triangleIndices[3] = triPoints[2];
                        triangleIndices[4] = triPoints[3];
                        triangleIndices[5] = triPoints[0];

                        AddFunction(triangleIndices);
                    }
                    else {
                        triangleIndices[0] = triPoints[0];
                        triangleIndices[1] = triPoints[3];
                        triangleIndices[2] = triPoints[2];
                        triangleIndices[3] = triPoints[2];
                        triangleIndices[4] = triPoints[1];
                        triangleIndices[5] = triPoints[0];

                        AddFunction(triangleIndices);
                    }
                }
            }
        }
    }
}

FRuntimeMeshCollisionSettings UChunkMeshProvider::GetCollisionSettings() {
    FRuntimeMeshCollisionSettings Settings;
    Settings.bUseAsyncCooking = false;

    return Settings;
}

bool UChunkMeshProvider::HasCollisionMesh() { 
    if (chunk->GetState() < ChunkState::HasOctree)
        return false;

    return true; 
}

bool UChunkMeshProvider::GetCollisionMesh(FRuntimeMeshCollisionData& CollisionData) {
    if (chunk->GetState() < ChunkState::HasOctree)
        return false;
    
    TMap<FIntVector8, int> indexMap;
    GenerateVerts(indexMap, 0, false, [&](FVector Location, FVector Normal) {
        CollisionData.Vertices.Add(FVector3f(Location));
    });
    GenerateTris(indexMap, 0, [&](int32 TriangleIndices[6]) {
        CollisionData.Triangles.Add(TriangleIndices[0], TriangleIndices[1], TriangleIndices[2]);
        CollisionData.Triangles.Add(TriangleIndices[3], TriangleIndices[4], TriangleIndices[5]);
    });

    return CollisionData.Vertices.Num() >= 3 && CollisionData.Triangles.Num() >= 3;
}
