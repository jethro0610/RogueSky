// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AllocatorNode.generated.h"

USTRUCT()
struct FAllocatorEdge {
	GENERATED_BODY()
	UAllocatorNode* nodes[2];
};

USTRUCT()
struct FAllocatorTriangle {
	GENERATED_BODY()
	FAllocatorEdge* edges[3];
};

UCLASS()
class ROGUESKY_API UAllocatorNode : public UObject
{
	GENERATED_BODY()
	
private:
	FVector2D location;
	float radius;
	float minDistance;

public:
	UFUNCTION(BlueprintCallable)
		void Initialize(FVector2D Location, float Radius, float MinDistance) {
			location = Location;
			radius = Radius;
			minDistance = MinDistance;
		}

	UFUNCTION(BlueprintCallable)
		bool SolveCollision(UAllocatorNode* Other);

	UFUNCTION(BlueprintPure)
		FVector2D GetLocation() const { return location; }

	UFUNCTION(BlueprintPure)
		float GetRadius() const { return radius; }
};
