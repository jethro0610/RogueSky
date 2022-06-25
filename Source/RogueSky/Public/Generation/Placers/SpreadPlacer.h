// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Generation/Generators/IslandGenerator.h"
#include "Gameplay/SpreadPoint.h"
#include "SpreadPlacer.generated.h"


UCLASS(Blueprintable, BlueprintType)
class ROGUESKY_API USpreadPlacer : public UObject {
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
		void Initialize(UIslandGenerator* Generator);
	UFUNCTION(BlueprintCallable)
		void Place();

private:
	UIslandGenerator* generator;
	FVector2D startPoint;
	FVector2D endPoint;
	float distanceBetweenPlacements;
};
