// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Generation/Allocators/AllocatorNode.h"
#include "AllocatorGraph.generated.h"

UCLASS()
class ROGUESKY_API UAllocatorGraph : public UObject
{
	GENERATED_BODY()

private:
	TArray<FAllocatorEdge> edges;
};
