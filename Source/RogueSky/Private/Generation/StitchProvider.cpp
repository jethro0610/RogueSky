// Fill out your copyright notice in the Description page of Project Settings.


#include "Generation/StitchProvider.h"
#include "Generation/Chunk.h"
#include "Generation/ChunkManager.h"
#include "Generation/GenerationConstants.h"
using namespace GenerationConstants;

void UStitchProvider::Initialize() {
    TArray<FRuntimeMeshLODProperties> lodProperties;
    lodProperties.Init(FRuntimeMeshLODProperties(), 1);
    lodProperties[0].ScreenSize = 0.75f;
    //lodProperties[1].ScreenSize = 0.25f;
    //lodProperties[2].ScreenSize = 0.05f;

    ConfigureLODs(lodProperties);
    SetupMaterialSlot(0, FName("Chunk Material"), GetStitchMaterial());

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

FBoxSphereBounds UStitchProvider::GetBounds() {
    FVector boxMin = chunk->GetLocation().ToVector() * CHUNK_SIZE;
    FVector boxMax = (chunk->GetLocation() + FIntVector8(1, 1, 1)).ToVector() * CHUNK_SIZE;
    return FBoxSphereBounds(FBox(boxMin, boxMax));
}

bool UStitchProvider::GetSectionMeshForLOD(int32 LODIndex, int32 SectionID, FRuntimeMeshRenderableMeshData& MeshData) {
    check(SectionID == 0);
    if (chunk->GetState() < ChunkState::HasOctree)
        return false;

    auto vertexFunction = ([&](FVector Location, FVector Normal) {
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
    auto triFunction = ([&](int32 TriangleIndices[3]) {
        for (int i = 0; i < 3; i++)
            MeshData.Triangles.Add(TriangleIndices[i]);
    });

    int vertCount = 0;
    GenerateStitchVertsAndTris(LODIndex, true, vertCount, vertexFunction, triFunction);
    GenerateCornerVertsAndTris(LODIndex, true, vertCount, vertexFunction, triFunction);

    MeshData.Colors.SetNum(MeshData.Positions.Num());
    MeshData.TexCoords.SetNum(MeshData.Positions.Num());

    chunk->SetState(ChunkState::HasStitch);
    return true;
}

Chunk* UStitchProvider::GetChunk() {
    FScopeLock(&this->chunkMutex);
    return chunk;
}
void UStitchProvider::SetChunk(Chunk* Chunk) {
    FScopeLock(&this->chunkMutex);
    chunk = Chunk;
}
UMaterialInterface* UStitchProvider::GetStitchMaterial() {
    FScopeLock(&this->chunkMutex);
    return material;
}
void UStitchProvider::SetStitchMaterial(UMaterialInterface* Material) {
    FScopeLock(&this->chunkMutex);
    material = Material;
}

void UStitchProvider::GenerateStitchVertsAndTris(uint8 LODLevel, bool CalculateNormals, int& VertCount, TFunctionRef<void(FVector Location, FVector Normal)> VertexFunction, TFunctionRef<void(int32 TriangleIndicies[3])> TriFunction) {
    const int lodChunkResolution = FMath::Exp2(MAX_LOD_DEPTH - LODLevel);
    for (int s = 0; s < 3; s++) { // Check all negative chunk borders
        // Skip this stitch if there's no adjacent chunk at the border
        const Chunk* oppositeChunk = chunk->GetChunkManager()->GetChunk(chunk->GetLocation() + stitchTable[s]);
        if (oppositeChunk == nullptr)
            continue;

        // Determine which direction the loop should run on
        // operates by setting that direction to a pointer and
        // adding to it every loop
        FIntVector8 origin(0, 0, 0);
        int8* a = nullptr;
        int8* b = nullptr;
        switch (s) {
        case 0:
            a = &origin.Y;
            b = &origin.Z;
            break;

        case 1:
            a = &origin.X;
            b = &origin.Z;
            break;

        case 2:
            a = &origin.X;
            b = &origin.Y;
            break;
        }

        for (*a = 0; *a < lodChunkResolution; *a += 1) {
            for (*b = 0; *b < lodChunkResolution; *b += 1) {
                // Get the distance and make zeroes positive
                FFloat16 e1 = chunk->distanceField.GetDistance(origin, LODLevel);
                if (e1 == 0.0f)
                    e1 = 1.0f;

                for (int e = 0; e < 3; e++) { // Check the edges of every vertex's cell
                    // Skip edges parallel to the stitch
                    if (e == s)
                        continue;

                    FIntVector8 e2Loc = origin + triangleEdgeTable[e];
                    FFloat16 e2 = chunk->distanceField.GetDistance(e2Loc, LODLevel);

                    // Make all zeroes positive
                    if (e2 == 0)
                        e2 = 1;

                    // If there's a sign change, create a plane perpidicular to the edge
                    if (e1 * e2 < 0) {
                        TArray<int> triPoints;
                        TSet<FVector> passedPoints;
                        for (int t = 0; t < 4; t++) {
                            FIntVector8 triPoint = origin + triangulationTable[e][t];

                            // Default to this chunk for the triangle point
                            const Chunk* triChunk = chunk;

                            // Get the crossing required to find a chunk
                            // determined if the point is negative, since stitches are
                            // only done in a negative axis direction
                            int crossings = 0;
                            if (triPoint.X < 0)
                                crossings++;
                            if (triPoint.Y < 0)
                                crossings++;
                            if (triPoint.Z < 0)
                                crossings++;

                            // Skip if two chunk crosses are required. 
                            // Set the chunk to use to the opposite if one chunk crossing is
                            // required since the only chunk that's ever crossed to is the opposite
                            if (crossings > 1)
                                continue;
                            if (crossings > 0) {
                                triChunk = oppositeChunk;
                                triPoint = triPoint + stitchTable[s] * -(lodChunkResolution);
                            }
                            const FVector vertexLocation = triChunk->GetVertexLocation(triPoint, LODLevel);
                            // Get the vertex and skip if it doesn't exist
                            if (vertexLocation == FVector::ZeroVector)
                                continue;

                            // Skip if the point has been passed already
                            if (passedPoints.Contains(vertexLocation))
                                continue;
                            passedPoints.Add(vertexLocation);

                            triPoints.Add(VertCount);
                            VertCount++;

                            FVector normal = FVector::ZeroVector;
                            if (CalculateNormals)
                                normal = triChunk->distanceField.GetNormal(triPoint, LODLevel);

                            VertexFunction(vertexLocation, normal);
                        }

                        // Generate the plane/tri
                        if (e1 > e2) {
                            if (triPoints.Num() >= 3) {
                                int32 triangleIndices[3];
                                triangleIndices[0] = triPoints[0];
                                triangleIndices[1] = triPoints[1];
                                triangleIndices[2] = triPoints[2];
                                TriFunction(triangleIndices);
                            }

                            if (triPoints.Num() == 4) {
                                int32 triangleIndices[3];
                                triangleIndices[0] = triPoints[2];
                                triangleIndices[1] = triPoints[3];
                                triangleIndices[2] = triPoints[0];
                                TriFunction(triangleIndices);
                            }
                        }
                        else {
                            if (triPoints.Num() >= 3) {
                                int32 triangleIndices[3];
                                triangleIndices[0] = triPoints[2];
                                triangleIndices[1] = triPoints[1];
                                triangleIndices[2] = triPoints[0];
                                TriFunction(triangleIndices);
                            }

                            if (triPoints.Num() == 4) {
                                int32 triangleIndices[3];
                                triangleIndices[0] = triPoints[0];
                                triangleIndices[1] = triPoints[3];
                                triangleIndices[2] = triPoints[2];
                                TriFunction(triangleIndices);
                            }
                        }
                    }
                }
            }
        }
    }
}

void UStitchProvider::GenerateCornerVertsAndTris(uint8 LODLevel, bool CalculateNormals, int& VertCount, TFunctionRef<void(FVector Location, FVector Normal)> VertexFunction, TFunctionRef<void(int32 TriangleIndicies[3])> TriFunction) {
    const int lodChunkResolution = FMath::Exp2(MAX_LOD_DEPTH - LODLevel);
    for (int c = 0; c < 3; c++) { // Loop through every corner of the chunk
        // Get all the chunks on the corner edges using the planar triangulation table
        const Chunk* cornerChunks[4] = { nullptr };
        for (int t = 0; t < 4; t++)
            cornerChunks[t] = chunk->GetChunkManager()->GetChunk(chunk->GetLocation() + triangulationTable[c][t]);

        // Loop through along the corner edge until the max length
        FIntVector8 origin = FIntVector8(0, 0, 0);
        for (int i = 0; i < lodChunkResolution; i++, origin += triangleEdgeTable[c]) {
            // Detect a sign change and skip if there is none
            FFloat16 e1 = chunk->distanceField.GetDistance(origin, LODLevel);
            if (e1 == 0.0f)
                e1 = 1.0f;
            FIntVector8 e2Loc = origin + triangleEdgeTable[c];
            FFloat16 e2 = chunk->distanceField.GetDistance(e2Loc, LODLevel);
            if (e2 == 0.0f)
                e2 = 1.0f;
            if (e1 * e2 >= 0.0f)
                continue;

            // Get all the points required to form the plane/tri
            TArray<int> triPoints;
            for (int t = 0; t < 4; t++) {
                if (cornerChunks[t] == nullptr)
                    continue;

                // Get the location of the chunk point needed to form the plane/tri
                // if any of the values are less than 0, then an adjacent chunk is needed
                // with its chunk point location at the edge
                FIntVector8 chunkPoint = origin + triangulationTable[c][t];
                if (chunkPoint.X < 0)
                    chunkPoint.X = lodChunkResolution - 1;
                if (chunkPoint.Y < 0)
                    chunkPoint.Y = lodChunkResolution - 1;
                if (chunkPoint.Z < 0)
                    chunkPoint.Z = lodChunkResolution - 1;

                const FVector vertexLocation = cornerChunks[t]->GetVertexLocation(chunkPoint, LODLevel);
                if (vertexLocation == FVector::ZeroVector)
                    continue;

                triPoints.Add(VertCount);
                VertCount++;

                FVector normal = FVector::ZeroVector;
                if (CalculateNormals)
                    normal = cornerChunks[t]->distanceField.GetNormal(chunkPoint, LODLevel);

                VertexFunction(vertexLocation, normal);
            }

            // Form the plane/tri
            if (e1 > e2) {
                if (triPoints.Num() >= 3) {
                    int32 triangleIndices[3];
                    triangleIndices[0] = triPoints[0];
                    triangleIndices[1] = triPoints[1];
                    triangleIndices[2] = triPoints[2];
                    TriFunction(triangleIndices);
                }

                if (triPoints.Num() == 4) {
                    int32 triangleIndices[3];
                    triangleIndices[0] = triPoints[2];
                    triangleIndices[1] = triPoints[3];
                    triangleIndices[2] = triPoints[0];
                    TriFunction(triangleIndices);
                }
            }
            else {
                if (triPoints.Num() >= 3) {
                    int32 triangleIndices[3];
                    triangleIndices[0] = triPoints[2];
                    triangleIndices[1] = triPoints[1];
                    triangleIndices[2] = triPoints[0];
                    TriFunction(triangleIndices);
                }

                if (triPoints.Num() == 4) {
                    int32 triangleIndices[3];
                    triangleIndices[0] = triPoints[0];
                    triangleIndices[1] = triPoints[3];
                    triangleIndices[2] = triPoints[2];
                    TriFunction(triangleIndices);
                }
            }
        }
    }
}


FRuntimeMeshCollisionSettings UStitchProvider::GetCollisionSettings() {
    FRuntimeMeshCollisionSettings Settings;
    Settings.bUseAsyncCooking = true;

    return Settings;
}

bool UStitchProvider::HasCollisionMesh() { 
    if (chunk->GetState() < ChunkState::HasOctree)
        return false;

    return true; 
}

bool UStitchProvider::GetCollisionMesh(FRuntimeMeshCollisionData& CollisionData) {
    if (chunk->GetState() < ChunkState::HasOctree)
        return false;

    auto vertexFunction = ([&](FVector Location, FVector Normal) {
        CollisionData.Vertices.Add(FVector3f(Location));
    });
    auto triFunction = ([&](int32 TriangleIndices[3]) {
        CollisionData.Triangles.Add(TriangleIndices[0], TriangleIndices[1], TriangleIndices[2]);
    });

    int vertCount = 0;
    GenerateStitchVertsAndTris(0, false, vertCount, vertexFunction, triFunction);
    GenerateCornerVertsAndTris(0, false, vertCount, vertexFunction, triFunction);

    return CollisionData.Vertices.Num() >= 3 && CollisionData.Triangles.Num() >= 3;
}
