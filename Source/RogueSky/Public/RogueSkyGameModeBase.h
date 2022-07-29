// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Generation/Chunk/ChunkManager.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Generation/LevelSections/LevelGenerator.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "RogueSkyGameModeBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FFixedTickDelegate);

UCLASS()
class ROGUESKY_API ARogueSkyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARogueSkyGameModeBase();

private:
	// Called when the game starts or when spawned
	void PreInitializeComponents() override;

	void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	AChunkManager* chunkManager;
	ALevelGenerator* levelGenerator;

	FVector2D elasticFieldOrigin;
	float elasticFieldSize = 18432.0f;

	UPROPERTY(EditAnywhere)
		UTextureRenderTarget2D* currentVelocityRT;
	UPROPERTY(EditAnywhere)
		UTextureRenderTarget2D* velocityBufferRT;

	UPROPERTY(EditAnywhere)
		UTextureRenderTarget2D* currentPositionRT;
	UPROPERTY(EditAnywhere)
		UTextureRenderTarget2D* positionBufferRT;

	UPROPERTY(EditAnywhere)
		UMaterial* updateVelocityMaterial;
	UPROPERTY(EditAnywhere)
		UMaterial* updatePositionMaterial;

	UPROPERTY(EditAnywhere)
		UMaterialInstance* copyToBufferMaterial;
	UPROPERTY()
		UMaterialInstanceDynamic* copyToBufferDynamic;

	UPROPERTY(EditAnywhere)
		UMaterialParameterCollection* elasticFieldCollection;

	UMaterialParameterCollectionInstance* elasticFieldProperties;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> playerClass;

	AActor* player = nullptr;
	ALevelSection* currentSection = nullptr;

private:
	UFUNCTION()
		void OnDoneGenerating();

	UFUNCTION()
		void OnPlayerOverlap(UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

public:
	UFUNCTION(BlueprintPure)
		AChunkManager* GetChunkManager() const { return chunkManager; }

	UFUNCTION(BlueprintPure)
		ALevelGenerator* GetLevelGenerator() const { return levelGenerator; }

	UFUNCTION(BlueprintCallable)
		void SetElasticFieldOrigin(FVector2D Location);

	UFUNCTION(BlueprintPure)
		void GetPositionInElasticField(FVector2D Location, bool& bIsInField, FVector2D& PositionInField) const;

	UFUNCTION(BlueprintPure)
		float GetElasticFieldSize() const { return elasticFieldSize; }

	UFUNCTION(BlueprintPure)
		UTextureRenderTarget2D* GetVelocityBuffer() const { return velocityBufferRT; }
	UFUNCTION(BlueprintPure)
		UTextureRenderTarget2D* GetPositionBuffer() const { return positionBufferRT; }

	UFUNCTION(BlueprintPure)
		UTextureRenderTarget2D* GetCurrentVelocityRT() const { return currentVelocityRT; }
	UFUNCTION(BlueprintPure)
		UTextureRenderTarget2D* GetCurrentPositionRT() const { return currentPositionRT; }

	UFUNCTION(BlueprintCallable)
		void SetCurrentSection(ALevelSection* Section);
};
