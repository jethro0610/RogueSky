#pragma once
// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreMinimal.h"
#include "Generation/DataTypes/GenerationTypes.h"
#include "Async/AsyncWork.h"

class Chunk;

/**
 *
 */
class GenerateVertexLocationsAsyncTask : public FNonAbandonableTask {
	friend class FAutoDeleteAsyncTask<GenerateVertexLocationsAsyncTask>;

	Chunk* chunk;
	const FIntVector8 chunkLocation;

	FORCEINLINE TStatId GetStatId() const {
		RETURN_QUICK_DECLARE_CYCLE_STAT(GenerateVertexLocationsAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}

	void DoWork();

public:
	GenerateVertexLocationsAsyncTask(Chunk* Chunk);
};
