// Fill out your copyright notice in the Description page of Project Settings.

#include "Generation/Generators/Generator.h"
#include "Generation/ChunkManager.h"
#include "Generation/DistanceFieldFunctions.h"
#include "Generation/GenerationTypes.h"

using namespace DistanceFieldFunctions;

// Sets default values
AGenerator::AGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(sceneComponent);
}

// Called when the game starts or when spawned
void AGenerator::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!generated)
		Generate();
}

void AGenerator::Generate() {
	const float radius = 8192.0f;
	const float perlinNoiseScale = 3500.0f;
	const float maxTerrainHeight = 1000.0f;
	const float maxUnderHeight = 8192.0f;

	AChunkManager* chunkManager = GetWorld()->GetAuthGameMode<ARogueSkyGameModeBase>()->GetChunkManager();
	FVector worldOrigin = GetActorLocation();

	TMap<FIntVector8, DistanceFieldContainer2D> chunkMask = GenerateChunkMaskAtLocation(worldOrigin, radius);
	int16 distanceField[DISTANCE_FIELD_SIZE][DISTANCE_FIELD_SIZE][DISTANCE_FIELD_SIZE];

	// Iterate through the mask chunks with terrain
	for (auto &pair : chunkMask) {
		bool isFilledChunk = false;

		// Iterate through each Z chunk of the mask chunks
		for (int chunkZ = FMath::FloorToInt((worldOrigin.Z - maxUnderHeight) / CHUNK_SIZE) - 1; chunkZ <= FMath::FloorToInt((worldOrigin.Z + maxTerrainHeight) / CHUNK_SIZE) + 1; chunkZ++) {
			FIntVector8 chunkLocation(pair.Key.X, pair.Key.Y, chunkZ);
			FVector chunkWorldOrigin = chunkLocation.ToVector() * CHUNK_SIZE;
			for (int x = 0; x < DISTANCE_FIELD_SIZE; x++)
			for (int y = 0; y < DISTANCE_FIELD_SIZE; y++) {
				const float scalar = FMath::Clamp(FMath::Abs(pair.Value.distanceField[x][y]), 0.0f, 2048.0f) / 2048.0f;
				const float terrainHeight = GetWorldPerlinHeight(FVector2D(pair.Key.X, pair.Key.Y), FVector2D(x, y), worldOrigin.Z, maxTerrainHeight, perlinNoiseScale, scalar);
				const float underHeight = GetWorldPerlinHeight(FVector2D(pair.Key.Y, pair.Key.Y), FVector2D(x, y), worldOrigin.Z, -maxUnderHeight, perlinNoiseScale / 4.0f, scalar);

				for (int z = 0; z < DISTANCE_FIELD_SIZE; z++) {
					// Get the mask distance from the map
					const float maskDistance = pair.Value.distanceField[x][y];

					const float zLocation = chunkWorldOrigin.Z + z * (CHUNK_SIZE / MAX_CHUNK_RESOLUTION);
					const float terrainDistance = FMath::Clamp(zLocation - terrainHeight, -(float)CHUNK_SIZE, (float)CHUNK_SIZE);
					const float underDistance = FMath::Clamp(underHeight - (zLocation + 2048.0f), -(float)CHUNK_SIZE, (float)CHUNK_SIZE);

					// Intersect the distacnes and mark the chunk as filled if the value isn't maxed
					float distance = SmoothMax(terrainDistance / CHUNK_SIZE, maskDistance / CHUNK_SIZE, 0.25f);
					distance = SmoothMax(distance, underDistance / CHUNK_SIZE, 0.25f);
					distanceField[x][y][z] = distance * INT16_MAX;
					if (distance < CHUNK_SIZE)
						isFilledChunk = true;
				}
			}
			// Add the chunk and distance field
			if (isFilledChunk) {
				Chunk* chunk = chunkManager->GetChunk(chunkLocation);
				if (chunk != nullptr) {
					//chunk->SetDistanceField(distanceField);
					chunk->SetState(ChunkState::HasDistanceField);
				}
			}
		}
	}

	generated = true;
}

TMap<FIntVector8, DistanceFieldContainer2D> AGenerator::GenerateChunkMaskAtLocation(FVector Location, float Radius) {
	// Generate the mask and get the chunks it occupies
	TMap<FIntVector8, DistanceFieldContainer2D> mask;

	// Iterate through each 2D chunk
	for (int chunkX = FMath::FloorToInt((Location.X - Radius) / CHUNK_SIZE) - 1; chunkX <= FMath::FloorToInt((Location.X + Radius) / CHUNK_SIZE) + 1; chunkX++)
	for (int chunkY = FMath::FloorToInt((Location.Y - Radius) / CHUNK_SIZE) - 1; chunkY <= FMath::FloorToInt((Location.Y + Radius) / CHUNK_SIZE) + 1; chunkY++) {
		const FIntVector8 chunkLocation(chunkX, chunkY);
		const FVector2D chunkWorldOrigin = FVector2D(chunkX, chunkY) * CHUNK_SIZE;
		// Iterate the distance fields of the chunks
		for (int x = 0; x < DISTANCE_FIELD_SIZE; x++)
			for (int y = 0; y < DISTANCE_FIELD_SIZE; y++) {
				const FVector2D worldLocation = chunkWorldOrigin + FVector2D(x, y) * (CHUNK_SIZE / MAX_CHUNK_RESOLUTION);
				const float distance = FMath::Clamp((worldLocation - FVector2D(Location.X, Location.Y)).Size() - Radius, -(float)CHUNK_SIZE, (float)CHUNK_SIZE);

				// Only add the chunk and distance if its value is less than the distance
				if (distance == CHUNK_SIZE)
					continue;

				if (!mask.Contains(chunkLocation))
					mask.Add(chunkLocation);

				mask[chunkLocation].distanceField[x][y] = distance;
			}
	}

	return mask;
}

float AGenerator::GetWorldPerlinHeight(FVector2D ChunkLocation, FVector2D ChunkPoint, float StartHeight, float MaxHeight, float NoiseScale, float Scalar) {
	const FVector2D worldLocation = ChunkLocation * CHUNK_SIZE + ChunkPoint * (CHUNK_SIZE / MAX_CHUNK_RESOLUTION);
	const float perlinHeight = (FMath::PerlinNoise2D(worldLocation / NoiseScale) + 1.0f) / 2.0f;
	return StartHeight + perlinHeight * MaxHeight * Scalar;
}
