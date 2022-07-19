// Fill out your copyright notice in the Description page of Project Settings.

#include "Generation/Chunk/ChunkManager.h"
#include "DrawDebugHelpers.h"
#include "Generation/Chunk/ChunkMeshProvider.h"
#include "Generation/Chunk/StitchProvider.h"
#include "Generation/DataTypes/DistanceField.h"

// Sets default values
AChunkManager::AChunkManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AChunkManager::BeginPlay() {
	Super::BeginPlay();
	Chunk::SetChunkManager(this);

	// Create the chunks	
	for (int8 x = -CHUNK_BOUNDS; x < CHUNK_BOUNDS; x++)
	for (int8 y = -CHUNK_BOUNDS; y < CHUNK_BOUNDS; y++)
	for (int8 z = -Z_CHUNK_BOUNDS; z < Z_CHUNK_BOUNDS; z++) {
		CreateChunk(x, y, z);
	}
}

void AChunkManager::BeginDestroy() {
	Super::BeginDestroy();
	Chunk::ClearChunkManager();
	for (int8 X = 0; X < CHUNK_COUNT; X++)
	for (int8 Y = 0; Y < CHUNK_COUNT; Y++)
	for (int8 Z = 0; Z < Z_CHUNK_COUNT; Z++) {
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

	int chunkCount = 0;
	int activeCount = 0;
	// Update the chunks
	for (int8 x = 0; x < CHUNK_COUNT; x++)
	for (int8 y = 0; y < CHUNK_COUNT; y++)
	for (int8 z = 0; z < Z_CHUNK_COUNT; z++) {
		chunks[x][y][z]->UpdateChunk();
		chunkCount++;
		if (chunks[x][y][z]->GetState() != ChunkState::Init && chunks[x][y][z]->GetState() != ChunkState::HasStitch)
			activeCount++;
	}

	if (activeCount == 0 && bGenerated == false) {
		UE_LOG(LogTemp, Warning, TEXT("Done generating"));
		OnDoneGenerating.Broadcast();
		bGenerated = true;
	}
}

Chunk* AChunkManager::CreateChunk(int8 X, int8 Y, int8 Z) {
	// @TODO: Hide any chunks that are invisible (nothing in the chunks). This will fix framerate dip from every chunk component rendering at a distance

	// Create the runtime mesh for the chunk
	URuntimeMeshComponent* newRuntimeMesh = NewObject<URuntimeMeshComponent>(this);
	newRuntimeMesh->SetMobility(EComponentMobility::Stationary);
	newRuntimeMesh->RegisterComponent();
	meshes.Add(newRuntimeMesh);
	UChunkMeshProvider* meshProvider = NewObject<UChunkMeshProvider>(this);

	// Create the runtime stitch for the chunk
	URuntimeMeshComponent* newRuntimeStitch = NewObject<URuntimeMeshComponent>(this);
	newRuntimeStitch->SetMobility(EComponentMobility::Stationary);
	newRuntimeStitch->RegisterComponent();
	stitches.Add(newRuntimeMesh);
	UStitchProvider* stitchProvider = NewObject<UStitchProvider>(this);

	// Create the chunk and set its LOD to the lowest
	Chunk* chunk = new Chunk(FIntVector8(X, Y, Z), meshProvider, stitchProvider);
	chunks[X + CHUNK_BOUNDS][Y + CHUNK_BOUNDS][Z + Z_CHUNK_BOUNDS] = chunk;

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
	Z += Z_CHUNK_BOUNDS;

	if (X < 0 || Y < 0 || Z < 0)
		return nullptr;

	if (X >= CHUNK_COUNT || Y >= CHUNK_COUNT || Z >= Z_CHUNK_COUNT)
		return nullptr;

	return chunks[X][Y][Z];
}

FVector AChunkManager::GetNormal(FVector Location) const {
	auto chunkAndVoxelLocation = GenerationConstants::WorldLocationToChunkSpace(Location);
	Chunk* chunk = GetChunk(chunkAndVoxelLocation.Key);

	if (chunk == nullptr)
		return FVector::ZeroVector;

	return chunk->distanceField.GetNormal(chunkAndVoxelLocation.Value);
}

