// Fill out your copyright notice in the Description page of Project Settings.
#include "Generation/LevelSections/SkyIslandSection.h"
#include "Generation/DataTypes/DistanceFieldFunctions.h"
using namespace DistanceFieldFunctions;

ASkyIslandSection::ASkyIslandSection() {
	minRadius = 12288.0f;
	maxRadius = 18432.0f;
}

void ASkyIslandSection::BeginPlay() {
	Super::BeginPlay();
}

void ASkyIslandSection::BeginDestroy() {
	for (auto pair : distanceFields) {
		delete pair.Value;
	}
	Super::BeginDestroy();
}

void ASkyIslandSection::Generate() {
	// Get a random point to sample for the large noise
	FVector2D largeOrigin;
	largeOrigin.X = FMath::Rand() - RAND_MAX / 2;
	largeOrigin.Y = FMath::Rand() - RAND_MAX / 2;

	// Initialize the 2D generators
	blobMask = BlobMask(GetLocation2D(), GetRadius() * 0.25, GetRadius(), properties.blobNoiseAmount);
	surface = TerrainHeight(
		GetActorLocation().Z + properties.minThickness / 2.0f,
		properties.surfaceMaxHeight,
		1.0f / properties.surfaceNoiseScale,
		largeOrigin,
		properties.sharedHeightRange,
		1.0f / properties.sharedNoiseScale
	);
	floor = TerrainHeight(
		GetActorLocation().Z - properties.minThickness / 2.0f,
		-properties.floorMaxHeight,
		1.0f / properties.floorNoiseScale,
		largeOrigin,
		properties.sharedHeightRange,
		1.0f / properties.sharedNoiseScale
	);

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

bool ASkyIslandSection::LocationIsInside(FVector2D Location, float DistanceToEdge) const {
	return blobMask.PointIsInBlob(Location, DistanceToEdge);
}

FVector ASkyIslandSection::GetLocationOnSurface(FVector2D Point) const {
	float height = surface.GetHeight(Point);
	return FVector(Point.X, Point.Y, height);
}

float ASkyIslandSection::GetEdgeDistanceFromOrigin(FVector2D EdgeDirection) const {
	return blobMask.GetEdgeDistanceFromOrigin(EdgeDirection);
}