// Fill out your copyright notice in the Description page of Project Settings.
#include "Generation/Generators/BlobMask.h"

BlobMask::BlobMask(FVector2D Origin, float MinRadius, float MaxRadius, float NoiseAmount) {
    origin = Origin;
    minRadius = MinRadius;
    maxRadius = MaxRadius;
    noiseAmount = NoiseAmount;

    noiseOrigin.X = FMath::Rand() - RAND_MAX / 2;
    noiseOrigin.Y = FMath::Rand() - RAND_MAX / 2;

    // Get the box of chunks the blobneeds to fit
    int8 startChunkX = FMath::FloorToInt((origin.X - maxRadius) / CHUNK_SIZE);
    int8 startChunkY = FMath::FloorToInt((origin.Y - maxRadius) / CHUNK_SIZE);
    startChunk = FIntPoint8(startChunkX, startChunkY);

    int8 endChunkX = FMath::FloorToInt((origin.X + maxRadius) / CHUNK_SIZE);
    int8 endChunkY = FMath::FloorToInt((origin.Y + maxRadius) / CHUNK_SIZE);
    endChunk = FIntPoint8(endChunkX, endChunkY);
}

BlobMask::~BlobMask() {

}

FFloat16 BlobMask::GetDistance(int8 ChunkX, int8 ChunkY, int8 VoxelX, int8 VoxelY) {
    // Get the distance of the voxel from the origin
    FVector2D point = ChunkCoordinateToWorld2D(ChunkX, ChunkY, VoxelX, VoxelY);
    float locationDistanceFromOrigin = FVector2D::Distance(origin, point);

    // Get the edge distance from the origin
    FVector2D vectorToLocation = (point - origin).GetSafeNormal();
    float edgeDistanceFromOrigin = GetEdgeDistanceFromOrigin(vectorToLocation);

    // The voxel's distance is the difference between the origin distance and edge distance;
    return locationDistanceFromOrigin - edgeDistanceFromOrigin;
}

float BlobMask::GetEdgeDistanceFromOrigin(FVector2D Direction) const {
    // Sample perlin noise in the same direction as the vector
    // This uses a unit vector to ensure a circular path is taken with the noise
    // Doing so results in a connected blob
    float noiseValue = (FMath::PerlinNoise2D(noiseOrigin + Direction * noiseAmount) + 1.0f) / 2.0f;
    return minRadius + noiseValue * (maxRadius - minRadius);
}

FVector2D BlobMask::GetRandomPoint(float MinDistanceToEdge) const {
    // Get a random direction
    float angle = FMath::Rand();
    FVector2D randomDirection(FMath::Cos(angle), FMath::Sin(angle));
    float edgeDistanceFromOrigin = GetEdgeDistanceFromOrigin(randomDirection);

    // Return the random direction vector with a randomized magnitude towards the edge of the blob
    float maxDistance = edgeDistanceFromOrigin - MinDistanceToEdge;
    float randomMagnitude = FMath::Rand() % (int)maxDistance;
    return origin + randomDirection * randomMagnitude;
}

bool BlobMask::PointIsInBlob(FVector2D Point, float MinDistanceToEdge) const {
    FVector2D vectorToPoint = FVector2D(Point - origin).GetSafeNormal();
    float locationDistanceFromOrigin = FVector2D::Distance(origin, Point);
    float edgeDistanceFromOrigin = GetEdgeDistanceFromOrigin(vectorToPoint);

    // NOTE: Edge limit may create uneven edges. Possible fix is to use the normal of the blob edge
    return locationDistanceFromOrigin < edgeDistanceFromOrigin - MinDistanceToEdge;
}
