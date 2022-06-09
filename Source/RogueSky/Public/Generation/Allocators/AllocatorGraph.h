// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Generation/Allocators/AllocatorNode.h"
#include "AllocatorGraph.generated.h"

USTRUCT(BlueprintType)
struct FAllocatorGraph {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FAllocatorEdge> edges;
};

UCLASS()
class UAllocatorGraphFunctions : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
		static void GetNodesInGraph(const FAllocatorGraph& Graph, TSet<UAllocatorNode*>& Nodes);

	UFUNCTION(BlueprintPure)
		static void GetEdgesConnectedToNode(const FAllocatorGraph& Graph, UAllocatorNode* Node, TArray<FAllocatorEdge>& OutEdges);

	UFUNCTION(BlueprintPure)
		static UAllocatorNode* GetNodeFromEdge(uint8 Index, const FAllocatorEdge& Edge) { return Edge.nodes[Index]; }

	UFUNCTION(BlueprintPure)
		static float GetEdgeLength(const FAllocatorEdge& Edge);

	UFUNCTION(BlueprintPure)
		static FAllocatorGraph GetMinimumSpanningTree(const FAllocatorGraph& Graph);
};
