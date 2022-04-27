// Fill out your copyright notice in the Description page of Project Settings.

#include "Generation/ChunkManager.h"
#include "DrawDebugHelpers.h"
#include "Generation/ChunkMeshProvider.h"
#include "Generation/StitchProvider.h"
#include "Generation/Generators/IslandGenerator.h"
#include "Generation/Placers/Placer.h"
#include "Generation/Placers/SpreadPlacer.h"

// Sets default values
AChunkManager::AChunkManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AChunkManager::BeginPlay()
{
	Super::BeginPlay();
	Chunk::SetChunkManager(this);

	// Create the chunks	
	for (int8 x = -CHUNK_BOUNDS; x < CHUNK_BOUNDS; x++)
	for (int8 y = -CHUNK_BOUNDS; y < CHUNK_BOUNDS; y++)
	for (int8 z = -CHUNK_BOUNDS; z < CHUNK_BOUNDS; z++) {
		CreateChunk(x, y, z);
	}

	IslandProperties islandProperties;
	islandProperties.origin = FVector::ZeroVector;
	IslandGenerator islandGenerator(islandProperties);
	islandGenerator.Generate();

	randomLocation = islandGenerator.GetRandomLocationOnSurface();

	for (auto pair : islandGenerator.distanceFields) {
		Chunk* chunk = GetChunk(pair.Key);
		if (chunk == nullptr)
			continue;

		chunk->distanceField.AddDistanceField(pair.Value);
		chunk->SetState(ChunkState::HasDistanceField);
	}
	
	SpreadPlacer placer;
	placer.startPoint = FVector2D(islandProperties.origin) - FVector2D(islandProperties.maxRadius, islandProperties.maxRadius);
	placer.endPoint = FVector2D(islandProperties.origin) + FVector2D(islandProperties.maxRadius, islandProperties.maxRadius);
	placer.actorToPlace = actorToPlace;
	placer.generator = &islandGenerator;
	placer.distanceBetweenPlacements = 150.0f;
	placer.world = GetWorld();

	placer.Place();
}

void AChunkManager::BeginDestroy() {
	Super::BeginDestroy();
	Chunk::ClearChunkManager();
	for (int8 X = 0; X < CHUNK_COUNT; X++)
	for (int8 Y = 0; Y < CHUNK_COUNT; Y++)
	for (int8 Z = 0; Z < CHUNK_COUNT; Z++) {
		delete chunks[X][Y][Z];
	}

	for (URuntimeMeshComponent* mesh : meshes)
		mesh->DestroyComponent();

	for (URuntimeMeshComponent* stitch : stitches)
		stitch->DestroyComponent();
};

// Called every frame
void AChunkManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update the chunks
	for (int8 x = 0; x < CHUNK_COUNT; x++)
	for (int8 y = 0; y < CHUNK_COUNT; y++)
	for (int8 z = 0; z < CHUNK_COUNT; z++) {
		chunks[x][y][z]->UpdateChunk();
	}

	//DrawDebugSphere(GetWorld(), randomLocation, 100.0f, 16.0f, FColor::Red);


}

Chunk* AChunkManager::CreateChunk(int8 X, int8 Y, int8 Z) {
	// Create the runtime mesh for the chunk
	URuntimeMeshComponent* newRuntimeMesh = NewObject<URuntimeMeshComponent>(this);
	newRuntimeMesh->RegisterComponent();
	meshes.Add(newRuntimeMesh);
	UChunkMeshProvider* meshProvider = NewObject<UChunkMeshProvider>(this);

	// Create the runtime stitch for the chunk
	URuntimeMeshComponent* newRuntimeStitch = NewObject<URuntimeMeshComponent>(this);
	newRuntimeStitch->RegisterComponent();
	stitches.Add(newRuntimeMesh);
	UStitchProvider* stitchProvider = NewObject<UStitchProvider>(this);

	// Create the chunk and set its LOD to the lowest
	Chunk* chunk = new Chunk(FIntVector8(X, Y, Z), meshProvider, stitchProvider);
	chunks[X + CHUNK_BOUNDS][Y + CHUNK_BOUNDS][Z + CHUNK_BOUNDS] = chunk;

	// Assign the chunk to the providers
	meshProvider->SetChunkMaterial(material);
	meshProvider->SetChunk(chunk);
	newRuntimeMesh->Initialize(meshProvider);
	stitchProvider->SetStitchMaterial(material);
	stitchProvider->SetChunk(chunk);
	newRuntimeStitch->Initialize(stitchProvider);

	return chunk;
}

Chunk* AChunkManager::GetChunk(int8 X, int8 Y, int8 Z) const {
	X += CHUNK_BOUNDS;
	Y += CHUNK_BOUNDS;
	Z += CHUNK_BOUNDS;

	if (X < 0 || Y < 0 || Z < 0)
		return nullptr;

	if (X >= CHUNK_COUNT || Y >= CHUNK_COUNT || Z >= CHUNK_COUNT)
		return nullptr;

	return chunks[X][Y][Z];
}


