// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GenerationConstants.h"
using namespace GenerationConstants;

class ROGUESKY_API DistanceField2D {
public:
	DistanceField2D();
	~DistanceField2D();

private:
	FFloat16 compressedDistanceField[DISTANCE_FIELD_SIZE][DISTANCE_FIELD_SIZE] = { CHUNK_SIZE };

public:
	FFloat16 GetDistance(int8 X, int8 Y) const { return compressedDistanceField[X][Y]; }
	FFloat16 GetDistance(FIntPoint8 Location) const { return GetDistance(Location.X, Location.Y); }

	void SetDistance(int8 X, int8 Y, FFloat16 Distance);
	void SetDistance(FIntPoint8 Location, FFloat16 Distance) { SetDistance(Location.X, Location.Y, Distance); }
};
