// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Generation/DistanceField2D.h"
#include "Generation/GenerationTypes.h"

/**
 * 
 */

class ROGUESKY_API BlobMask
{
public:
	BlobMask(FVector2D Origin = FVector2D::ZeroVector, float MinRadius = 1024.0f, float MaxRadius = 4096.0f, float NoiseAmount = 1.0f);
	~BlobMask();
	
private:
	FVector2D origin;
	float maxRadius;
	float minRadius;
	float noiseAmount;
	FVector2D noiseOrigin;

	FIntPoint8 startChunk;
	FIntPoint8 endChunk;

public:
	FFloat16 GetDistance(int8 ChunkX, int8 ChunkY, int8 VoxelX, int8 VoxelY);

	int8 GetStartChunkX() const { return startChunk.X; }
	int8 GetStartChunkY() const { return startChunk.Y; }
	int8 GetEndChunkX() const { return endChunk.X; }
	int8 GetEndChunkY() const { return endChunk.Y; }

	FVector2D GetOrigin() const { return origin; }
	float GetEdgeDistanceFromOrigin(FVector2D Direction) const;
	FVector2D GetRandomPoint(float MinDistanceToEdge = 0.0f) const;
	bool PointIsInBlob(FVector2D Point, float MinDistanceToEdge = 0.0f) const;
};
