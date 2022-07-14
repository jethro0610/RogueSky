// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "LevelSection.h"
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
	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<ALevelSection>> levelSections;

	TMap<UAllocatorNode*, ALevelSection*> nodeSectionMap;

	UPROPERTY(EditAnywhere)
		UStaticMesh* railMesh;

	UPROPERTY()
		UAllocator* allocator;

	UPROPERTY(EditAnywhere)
		float minDistance = 128.0f;
	UPROPERTY(EditAnywhere)
		float maxDistance = 256.0f;

public:
	UFUNCTION(BlueprintCallable)
		void GenerateLevel();

	UFUNCTION(BlueprintCallable)
		void GenerateConnections();

	UFUNCTION(BlueprintPure)
		UAllocator* GetAllocator() const { return allocator; }

	UFUNCTION(BlueprintPure)
		ALevelSection* GetSectionFromNode(UAllocatorNode* Node) { return nodeSectionMap[Node]; }
};
