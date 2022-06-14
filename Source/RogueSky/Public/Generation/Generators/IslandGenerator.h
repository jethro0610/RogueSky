// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Generation/DistanceField.h"
#include "Generation/GenerationTypes.h"
#include "Generation/Generators/BlobMask.h"
#include "Generation/Generators/TerrainHeight.h"

/**
 * 
 */

struct IslandProperties {
	FVector origin = FVector::ZeroVector;

	float minRadius = 1024.0f;
	float maxRadius = 16384.0f;
	float blobNoiseAmount = 1.5f;

	float minThickness = 2048.0f;

	float sharedHeightRange = 2048.0f;
	float sharedNoiseScale = 8192.0f;

	float surfaceMaxHeight = 512.0f;
	float surfaceNoiseScale = 3072.0f;

	float floorMaxHeight = 2048.0f;
	float floorNoiseScale = 4096.0f;
};

class ROGUESKY_API IslandGenerator {
public:
	IslandGenerator(IslandProperties Properties);
	~IslandGenerator();
	TMap<FIntVector8, DistanceField*> distanceFields;

private:
	IslandProperties properties;
	BlobMask blobMask;
	TerrainHeight surface;
	TerrainHeight floor;

public:
	BlobMask& GetBlobMask() { return blobMask; }
	TerrainHeight& GetSurface() { return surface; }

	void Generate();
	FVector GetLocationOnSurface(FVector2D Point) const;
	FVector GetRandomLocationOnSurface(float MinDistanceToEdge = 0.0f) const;
};
