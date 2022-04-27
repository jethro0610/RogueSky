// Fill out your copyright notice in the Description page of Project Settings.
#include "Generation/Generators/IslandGenerator.h"
#include "Generation/DistanceFieldFunctions.h"
using namespace DistanceFieldFunctions;

IslandGenerator::IslandGenerator(IslandProperties Properties) {
	properties = Properties;

	// Get a random point to sample for the large noise
	FVector2D largeOrigin;
	largeOrigin.X = FMath::Rand() - RAND_MAX / 2;
	largeOrigin.Y = FMath::Rand() - RAND_MAX / 2;

	// Initialize the 2D generators
	blobMask = BlobMask(FVector2D(properties.origin), properties.minRadius, properties.maxRadius, properties.blobNoiseAmount);
	surface = TerrainHeight(
		properties.origin.Z + properties.minThickness / 2.0f,
		properties.surfaceMaxHeight,
		1.0f / properties.surfaceNoiseScale,
		largeOrigin,
		properties.sharedHeightRange,
		1.0f / properties.sharedNoiseScale
	);
	floor = TerrainHeight(
		properties.origin.Z - properties.minThickness / 2.0f,
		-properties.floorMaxHeight,
		1.0f / properties.floorNoiseScale,
		largeOrigin,
		properties.sharedHeightRange,
		1.0f / properties.sharedNoiseScale
	);
}

IslandGenerator::~IslandGenerator() {
	for (auto pair : distanceFields) {
		delete pair.Value;
	}
}

void IslandGenerator::Generate() {
	// Iterate through 2D chunks the blob outlines
	// Iterate through the z chunks based on the terrain's range
	for (int8 chunkX = blobMask.GetStartChunkX(); chunkX <= blobMask.GetEndChunkX(); chunkX++)
	for (int8 chunkY = blobMask.GetStartChunkY(); chunkY <= blobMask.GetEndChunkY(); chunkY++)
	for (int8 chunkZ = floor.GetStartChunkZ(); chunkZ <= surface.GetEndChunkZ(); chunkZ++) {
		DistanceField* distanceField = new DistanceField;
		distanceFields.Add(FIntVector8(chunkX, chunkY, chunkZ), distanceField);

		// Fill the chunk's distance field with the blob's outline
		for (int8 x = 0; x < DISTANCE_FIELD_SIZE; x++)
		for (int8 y = 0; y < DISTANCE_FIELD_SIZE; y++) {
			FFloat16 blobDistance = blobMask.GetDistance(chunkX, chunkY, x, y);
			FFloat16 surfaceHeight = surface.GetHeight(chunkX, chunkY, x, y);
			FFloat16 floorHeight = floor.GetHeight(chunkX, chunkY, x, y);
			for (int8 z = 0; z < DISTANCE_FIELD_SIZE; z++) {
				int worldZPos = chunkZ * CHUNK_SIZE + z * VOXEL_SIZE;
				FFloat16 terrainDistance = SmoothMax(floorHeight - worldZPos, worldZPos - surfaceHeight, 0.2f);
				FFloat16 finalDistance = SmoothMax(blobDistance, terrainDistance, 0.5f);
				distanceField->SetDistance(x, y, z, finalDistance);
			}
		}
	}
}

FVector IslandGenerator::GetLocationOnSurface(FVector2D Point) const {
	float height = surface.GetHeight(Point);
	return FVector(Point.X, Point.Y, height);
}

FVector IslandGenerator::GetRandomLocationOnSurface(float MinDistanceToEdge) const {
	FVector2D randomPoint = blobMask.GetRandomPoint(MinDistanceToEdge);
	return GetLocationOnSurface(randomPoint);
}
