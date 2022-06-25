// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Generation/Generators/IslandGenerator.h"

/**
 * 
 */
class ROGUESKY_API SpreadPlacer {
public:
	SpreadPlacer();
	~SpreadPlacer();

public:
	TSubclassOf<AActor> actorToPlace;
	UWorld* world;
	UIslandGenerator* generator;

	FVector2D startPoint;
	FVector2D endPoint;
	float distanceBetweenPlacements;

	void Place();
};
