// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Generation/Generators/IslandGenerator.h"
#include "Generation/Allocators/Allocator.h"
#include "LevelGenerator.generated.h"
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ROGUESKY_API ALevelGenerator : public AActor {
	GENERATED_BODY()

public:
	ALevelGenerator();

private:
	UPROPERTY()
		TArray<UIslandGenerator*> islandGenerators;

	TMap<UAllocatorNode*, UIslandGenerator*> nodeIslandMap;

	UPROPERTY(EditAnywhere)
		UStaticMesh* railMesh;

	UPROPERTY(VisibleAnywhere)
		UAllocator* allocator;

	UPROPERTY(EditAnywhere)
		int numberOfIslands;
	UPROPERTY(EditAnywhere)
		float minRadius;
	UPROPERTY(EditAnywhere)
		float maxRadius;
	UPROPERTY(EditAnywhere)
		float minDistance;
	UPROPERTY(EditAnywhere)
		float maxDistance;

public:
	UFUNCTION(BlueprintCallable)
		void GenerateLevel();

	UFUNCTION(BlueprintCallable)
		void GenerateConnections();

	UFUNCTION(BlueprintPure)
		UAllocator* GetAllocator() const { return allocator; }

	UFUNCTION(BlueprintPure)
		UIslandGenerator* GetIslandFromNode(UAllocatorNode* Node) { return nodeIslandMap[Node]; }
};
