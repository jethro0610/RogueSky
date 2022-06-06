// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Generation/Allocators/AllocatorNode.h"
#include "Allocator.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, ClassGroup="Allocation")
class ROGUESKY_API UAllocator : public UObject
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
		TArray<UAllocatorNode*> nodes;

	UPROPERTY(EditAnywhere)
		float minRadius = 512.0f;

	UPROPERTY(EditAnywhere)
		float maxRadius = 1024.0f;

	UPROPERTY(EditAnywhere)
		float minDist = 128.0f;

	UPROPERTY(EditAnywhere)
		float maxDist = 256.0f;

	UPROPERTY(EditAnywhere)
		FVector2D origin = FVector2D::ZeroVector;

	UPROPERTY(EditAnywhere)
		float placementRange = 10.0f;

public:
	UFUNCTION(BlueprintCallable)
		void PlaceRandomNodes(int NumberOfNodes);

	UFUNCTION(BlueprintCallable)
		void AllocateNodes();

	UFUNCTION(BlueprintPure)
		TArray<UAllocatorNode*> GetNodes() const { return nodes; }
};
