// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Generation/Generators/IslandGenerator.h"

/**
 * 
 */
class ROGUESKY_API Placer {
public:
	Placer(TSubclassOf<AActor> ActorToPlace);
	~Placer();

	void Place(UIslandGenerator* Generator, UWorld* World);
	void PlaceRandom(UIslandGenerator* Generator, UWorld* World);

private:
	int maxPlacementsBeforeRotate = 4;
	int minPlacementsBeforeRotate = 2;
	float placementDistance = 3000.0f;
	int maxRotationAmount = 45.0f;
	TSubclassOf<AActor> actorToPlace;
};
