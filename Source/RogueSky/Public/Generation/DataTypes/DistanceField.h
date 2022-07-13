// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GenerationConstants.h"
#include "GenerationTypes.h"
using namespace GenerationConstants;

/**
 * 
 */
class ROGUESKY_API DistanceField
{
public:
	DistanceField();
	~DistanceField();

private:
	FFloat16 compressedDistanceField[DISTANCE_FIELD_SIZE][DISTANCE_FIELD_SIZE][DISTANCE_FIELD_SIZE] = { CHUNK_SIZE };

public:
	FFloat16 GetDistance(int8 X, int8 Y, int8 Z) const { return compressedDistanceField[X][Y][Z]; }
	FFloat16 GetDistance(int8 X, int8 Y, int8 Z, uint8 LODLevel) const;
	FFloat16 GetDistance(FIntVector8 Location) const { return GetDistance(Location.X, Location.Y, Location.Z); }
	FFloat16 GetDistance(FIntVector8 Location, uint8 LODLevel) const { return GetDistance(Location.X, Location.Y, Location.Z, LODLevel); }

	void SetDistance(int8 X, int8 Y, int8 Z, FFloat16 Distance);
	void SetDistance(FIntVector8 Location, FFloat16 Distance) { SetDistance(Location.X, Location.Y, Location.Z, Distance); }

	FVector GetNormal(int8 X, int8 Y, int8 Z) const;
	FVector GetNormal(int8 X, int8 Y, int8 Z, uint8 LODLevel) const;
	FVector GetNormal(FIntVector8 Location) const { return GetNormal(Location.X, Location.Y, Location.Z); }
	FVector GetNormal(FIntVector8 Location, uint8 LODLevel) const { return GetNormal(Location.X, Location.Y, Location.Z, LODLevel); }

	void AddDistanceField(DistanceField* DistanceFieldToAdd);
};
