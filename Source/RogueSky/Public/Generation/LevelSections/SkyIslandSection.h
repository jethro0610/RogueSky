// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "LevelSection.h"
#include "Generation/DataTypes/GenerationTypes.h"
#include "Generation/DataTypes/BlobMask.h"
#include "Generation/DataTypes/TerrainHeight.h"
#include "SkyIslandSection.generated.h"

USTRUCT()
struct FSkyIslandProperties {
	GENERATED_BODY()

	UPROPERTY()
		float blobNoiseAmount = 1.5f;
	UPROPERTY()
		float minThickness = 2048.0f;
	UPROPERTY()
		float sharedHeightRange = 2048.0f;
	UPROPERTY()
		float sharedNoiseScale = 8192.0f;
	UPROPERTY()
	float surfaceMaxHeight = 512.0f;
	UPROPERTY()
		float surfaceNoiseScale = 3072.0f;
	UPROPERTY()
		float floorMaxHeight = 2048.0f;
	UPROPERTY()
		float floorNoiseScale = 4096.0f;
};

UCLASS(Blueprintable, BlueprintType)
class ROGUESKY_API ASkyIslandSection : public ALevelSection {
	GENERATED_BODY()

public:
	ASkyIslandSection();

private:
	void BeginPlay() override;
	void BeginDestroy() override;

	UPROPERTY(EditAnywhere)
		FSkyIslandProperties properties;

	BlobMask blobMask;
	TerrainHeight surface;
	TerrainHeight floor;

public:
	void Generate() override;
	bool LocationIsInside(FVector2D Location, float DistanceToEdge = 0.0f) const override;
	FVector GetLocationOnSurface(FVector2D Location) const override;
	FVector GetSurfaceNormal(FVector2D Point) const override;
	float GetEdgeDistanceFromOrigin(FVector2D EdgeDirection) const override;
};
