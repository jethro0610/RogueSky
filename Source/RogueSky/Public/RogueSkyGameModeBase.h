// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Generation/Chunk/ChunkManager.h"
#include "Gameplay/Level/SpreadManager.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/KismetRenderingLibrary.h"
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

	FVector2D rtFieldOrigin;
	float rtFieldSize;

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

public:
	UFUNCTION(BlueprintPure)
		AChunkManager* GetChunkManager() const { return chunkManager; }

	UFUNCTION(BlueprintCallable)
		void SetRTFieldOrigin(FVector2D Location, float Size);

	UFUNCTION(BlueprintPure)
		FVector2D GetPositionInRTField(FVector2D Location, bool& bIsInField) const;

	UFUNCTION(BlueprintPure)
		UTextureRenderTarget2D* GetVelocityBuffer() const { return velocityBufferRT; }
	UFUNCTION(BlueprintPure)
		UTextureRenderTarget2D* GetPositionBuffer() const { return positionBufferRT; }

	UFUNCTION(BlueprintPure)
		UTextureRenderTarget2D* GetCurrentVelocityRT() const { return currentVelocityRT; }
	UFUNCTION(BlueprintPure)
		UTextureRenderTarget2D* GetCurrentPositionRT() const { return currentPositionRT; }
};
