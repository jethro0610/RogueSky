// Fill out your copyright notice in the Description page of Project Settings.
#include "Generation/Generators/TerrainHeight.h"
#include "Generation/GenerationConstants.h"
using namespace GenerationConstants;

TerrainHeight::TerrainHeight(float StartHeight, float MaxHeight, float NoiseAmount, FVector2D SharedNoiseOrigin, float SharedHeightRange, float SharedNoiseAmount) {
    startHeight = StartHeight;
    maxHeight = MaxHeight;
    noiseAmount = NoiseAmount;

    noiseOrigin.X = FMath::Rand() - RAND_MAX / 2;
    noiseOrigin.Y = FMath::Rand() - RAND_MAX / 2;

    sharedNoiseOrigin = SharedNoiseOrigin;
    sharedHeightScale = SharedHeightRange / 2.0f;
    sharedNoiseAmount = SharedNoiseAmount;

    if (maxHeight > 0)
        startChunkZ = FMath::FloorToInt((startHeight - sharedHeightScale) / CHUNK_SIZE) - 1;
    else
        startChunkZ = FMath::FloorToInt(((startHeight + maxHeight) - sharedHeightScale) / CHUNK_SIZE) - 1;

    if (maxHeight > 0)
        endChunkZ = FMath::FloorToInt(((startHeight + maxHeight) + sharedHeightScale) / CHUNK_SIZE) + 1;
    else
        endChunkZ = FMath::FloorToInt((startHeight + sharedHeightScale) / CHUNK_SIZE) + 1;
}

TerrainHeight::~TerrainHeight() {

}

FFloat16 TerrainHeight::GetHeight(int8 ChunkX, int8 ChunkY, int8 VoxelX, int8 VoxelY) const {
    FVector2D location = ChunkCoordinateToWorld2D(ChunkX, ChunkY, VoxelX, VoxelY);

    float sharedNoise = (FMath::PerlinNoise2D(sharedNoiseOrigin + location * sharedNoiseAmount));
    float sharedHeight = sharedNoise * sharedHeightScale;

    float surfaceNosie = (FMath::PerlinNoise2D(noiseOrigin + location * noiseAmount) + 1.0f) / 2.0f;
    float surfaceHeight = surfaceNosie * maxHeight;
    return startHeight + surfaceHeight + sharedHeight;
}

FFloat16 TerrainHeight::GetHeight(FVector2D Point) const {
    float sharedNoise = (FMath::PerlinNoise2D(sharedNoiseOrigin + Point * sharedNoiseAmount));
    float sharedHeight = sharedNoise * sharedHeightScale;

    float surfaceNosie = (FMath::PerlinNoise2D(noiseOrigin + Point * noiseAmount) + 1.0f) / 2.0f;
    float surfaceHeight = surfaceNosie * maxHeight;
    return startHeight + surfaceHeight + sharedHeight;
}