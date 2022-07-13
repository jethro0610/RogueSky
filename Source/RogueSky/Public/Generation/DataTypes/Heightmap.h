// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GenerationConstants.h"
using namespace GenerationConstants;

/**
 * 
 */
class ROGUESKY_API Heightmap {
public:
	Heightmap();
	~Heightmap();

private:
	FFloat16 heightmap[DISTANCE_FIELD_SIZE][DISTANCE_FIELD_SIZE];

public:
	FFloat16 GetHeight(int8 X, int8 Y) { return heightmap[X][Y]; }
	void SetHeight(int X, int Y, FFloat16 Height) { heightmap[X][Y] = Height; }
};
