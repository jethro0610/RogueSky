// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "Generation/GenerationTypes.h"
#include "Components/RuntimeMeshComponentStatic.h"
#include "ChunkMeshProvider.h"
#include "StitchProvider.h"
#include "Runtime/Core/Public/HAL/ThreadingBase.h"
#include "GenerationConstants.h"
#include "GenerateVertexLocationsAsyncTask.h"
#include "Generation/DistanceField.h"
using namespace GenerationConstants;

class AChunkManager;

enum ChunkState {
	Init,
	GeneratingDistanceField,
	HasDistanceField,
	DistanceFieldWait,
	GeneratingOctree,
	HasOctree,
	GeneratingMesh,
	HasMesh,
	GeneratingStitch,
	HasStitch,
};

class Chunk {
public:
	DistanceField distanceField;

	Chunk(FIntVector8 ChunkLocation, UChunkMeshProvider* MeshProvider, UStitchProvider* StitchProvider) {
		check(chunkManager != nullptr);
		meshProvider = MeshProvider;
		stitchProvider = StitchProvider;
		chunkState = Init;
		chunkLocation = ChunkLocation;
	}
	~Chunk();

private:
	static AChunkManager* chunkManager;
	UChunkMeshProvider* meshProvider;
	UStitchProvider* stitchProvider;
	ChunkState chunkState;
	FIntVector8 chunkLocation;
	TMap<FIntVector8, FVector> vertexLocations;
	FCriticalSection vertexMutex;

public:
	void UpdateChunk();
	void SetState(ChunkState State) { chunkState = State; }

	ChunkState GetState() const { return chunkState; }
	AChunkManager* GetChunkManager() const { return Chunk::chunkManager; }
	FIntVector8 GetLocation() const { return chunkLocation; }

	bool CanStitch() const;
	bool IsGenerating() const { return (chunkState == GeneratingOctree && chunkState == GeneratingMesh && chunkState == GeneratingStitch); }
	FString ChunkStateToString() const;

	FVector GetVertexLocation(int8 X, int8 Y, int8 Z) const;
	FVector GetVertexLocation(int8 X, int8 Y, int8 Z, uint8 LODLevel) const;
	FVector GetVertexLocation(FIntVector8 Location, uint8 LODLevel) const { return GetVertexLocation(Location.X, Location.Y, Location.Z, LODLevel); }
	void SetVertexLocation(int8 X, int8 Y, int8 Z, FVector VertexLocation);

public:
	static void SetChunkManager(AChunkManager* ChunkManager) {
		check(chunkManager == nullptr);
		chunkManager = ChunkManager;
	}
	static void ClearChunkManager() {
		chunkManager = nullptr;
	}
	void GenerateVertexLocationsAsync() {
		chunkState = GeneratingOctree;
		(new FAutoDeleteAsyncTask<GenerateVertexLocationsAsyncTask>(this))->StartBackgroundTask();
	}
};
