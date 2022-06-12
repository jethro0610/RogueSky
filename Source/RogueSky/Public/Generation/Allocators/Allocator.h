// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Generation/Allocators/AllocatorNode.h"
#include "Generation/Allocators/AllocatorGraph.h"
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

	UPROPERTY()
		FAllocatorGraph delauneyGraph;

	UPROPERTY()
		FAllocatorGraph minimumSpanningTree;

	const float SUPER_TRIANGLE_RANGE = 65536 * 2;

public:
	UFUNCTION(BlueprintCallable)
		void PlaceRandomNodes(int NumberOfNodes, float MinRadius, float MaxRadius, float MinDistance, float MaxDistance, FVector2D Origin);

	UFUNCTION(BlueprintCallable)
		void AllocateNodes();

	UFUNCTION(BlueprintCallable)
		void UpdateGraphs();

	UFUNCTION(BlueprintPure)
		TArray<UAllocatorNode*> GetNodes() const { return nodes; }

	UFUNCTION(BlueprintPure)
		FAllocatorGraph GetDelauneyGraph() const { return delauneyGraph; }

	UFUNCTION(BlueprintPure)
		FAllocatorGraph GetMinimumSpanningTree() const { return minimumSpanningTree; }
};
