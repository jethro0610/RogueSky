// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GenerationTypes.h"
#include "Heightmap.h"

/**
 * 
 */
class ROGUESKY_API TerrainHeight
{
public:
	TerrainHeight(
		float MinHeight = 0.0f, float MaxHeight = 512.0f, float NoiseAmount = 1.0f, 
		FVector2D SharedNoiseOrigin = FVector2D::ZeroVector, float SharedHeightRange = 0.0f, float SharedNoiseAmount = 1.0f
	);
	~TerrainHeight();

private:
	TArray<FIntPoint8> chunksToGenerateOn;
	float startHeight;
	float maxHeight;
	float noiseAmount;
	FVector2D noiseOrigin;

	FVector2D sharedNoiseOrigin;
	float sharedHeightScale;
	float sharedNoiseAmount;

	int8 startChunkZ;
	int8 endChunkZ;

public:
	FFloat16 GetHeight(int8 ChunkX, int8 ChunkY, int8 VoxelX, int8 VoxelY) const;
	FFloat16 GetHeight(FVector2D Point) const;
	int8 GetStartChunkZ() const { return startChunkZ; }
	int8 GetEndChunkZ() const { return endChunkZ; }
};
