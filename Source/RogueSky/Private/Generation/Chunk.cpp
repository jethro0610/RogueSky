// Fill out your copyright notice in the Description page of Project Settings.
#include "Generation/Chunk.h"
#include "Generation/ChunkManager.h"
#include "Generation/GenerationConstants.h"
#include "Generation/DistanceFieldFunctions.h"

using namespace GenerationConstants;
using namespace DistanceFieldFunctions;
AChunkManager* Chunk::chunkManager = nullptr;

Chunk::~Chunk() {

}

void Chunk::UpdateChunk() {
    switch (chunkState) {
    case HasDistanceField:
        chunkState = DistanceFieldWait;
        break;

    case DistanceFieldWait:
        GenerateVertexLocationsAsync();
        break;

    case HasOctree:
        chunkState = GeneratingMesh;
        meshProvider->UpdateMesh();
        break;

    case HasMesh: 
        if (CanStitch() && !IsGenerating()) {
            stitchProvider->UpdateStitch();
        }
        break;

    default:
        break;
    }
}

FVector Chunk::GetVertexLocation(int8 X, int8 Y, int8 Z) const {
    const FVector* vertexLocation = vertexLocations.Find(FIntVector8(X, Y, Z));
    if (vertexLocation != nullptr)
        return *vertexLocation;
    else
        return FVector::ZeroVector;
}

FVector Chunk::GetVertexLocation(int8 X, int8 Y, int8 Z, uint8 LODLevel) const {
    if (LODLevel == 0)
        return GetVertexLocation(X, Y, Z);

    int lodUnitSize = FMath::Exp2(LODLevel);

    FVector vectorSum = FVector::ZeroVector;
    int totalVectors = 0;
    for (int x = X * lodUnitSize; x < X * lodUnitSize + lodUnitSize; x++)
    for (int y = Y * lodUnitSize; y < Y * lodUnitSize + lodUnitSize; y++)
    for (int z = Z * lodUnitSize; z < Z * lodUnitSize + lodUnitSize; z++) {
        const FVector vertexLocation = GetVertexLocation(x, y, z);
        if (vertexLocation != FVector::ZeroVector) {
            vectorSum += vertexLocation;
            totalVectors++;
        }
    }

    if (totalVectors == 0)
        return FVector::ZeroVector;

    return vectorSum / totalVectors;
}

void Chunk::SetVertexLocation(int8 X, int8 Y, int8 Z, FVector VertexLocation) {
    vertexMutex.Lock();
    vertexLocations.Add(FIntVector8(X, Y, Z), VertexLocation);
    vertexMutex.Unlock();
}

bool Chunk::CanStitch() const {
    for (int x = 0; x > -2; x--) {
        for (int y = 0; y > -2; y--) {
            for (int z = 0; z > -2; z--) {
                const Chunk* adjacent = chunkManager->GetChunk(chunkLocation + FIntVector8(x, y, z));

                // Ignore non-existent chunks
                if (adjacent == nullptr)
                    continue;

                // Ignore idle chunks
                if (adjacent->chunkState == Init)
                    continue;

                // Don't generate stitch if the chunk has no verticies
                if (adjacent->GetState() < HasOctree)
                    return false;
            }
        }
    }

    return true;
}

FString Chunk::ChunkStateToString() const {
    switch (chunkState) {
    case Init:
        return "Init";

    case GeneratingDistanceField:
        return "Generating Distance Field";

    case HasDistanceField:
        return "Has Distance Field";

    case GeneratingOctree:
        return "Generating Octree";

    case HasOctree:
        return "Has Octree";

    case GeneratingMesh:
        return "Generating Mesh";

    case HasMesh:
        return "Has Mesh";

    case GeneratingStitch:
        return "Generating Stitch";

    case HasStitch:
        return "Has Stitch";

    default:
        return "NULL";
    }
}