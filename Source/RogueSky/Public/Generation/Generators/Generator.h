// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Generation/Chunk/Chunk.h"
#include "Generation/Chunk/ChunkManager.h"
#include "RogueSkyGameModeBase.h"
#include "Generation/DataTypes/GenerationConstants.h"
#include "Generator.generated.h"
using namespace GenerationConstants;

struct DistanceFieldContainer2D {
	float distanceField[DISTANCE_FIELD_SIZE][DISTANCE_FIELD_SIZE];

	DistanceFieldContainer2D() {
		for (int x = 0; x < DISTANCE_FIELD_SIZE; x++) {
			for (int y = 0; y < DISTANCE_FIELD_SIZE; y++) {
				distanceField[x][y] = 4096.0f;
			}
		}
	}
};

UCLASS()
class ROGUESKY_API AGenerator : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
		USceneComponent* sceneComponent;
	bool generated = false;

	TMap<FIntVector8, DistanceFieldContainer2D> GenerateChunkMaskAtLocation(FVector Location, float Radius); 
	float GetWorldPerlinHeight(FVector2D ChunkLocation, FVector2D ChunkPoint, float StartHeight, float MaxHeight, float NoiseScale, float Scalar = 1.0f);

protected:
	virtual void Generate();
};
