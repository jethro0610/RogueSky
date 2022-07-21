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

	FVector windOrigin;
	float windFieldSize;

	UPROPERTY(EditAnywhere)
		UTextureRenderTarget2D* windField;
	UPROPERTY(EditAnywhere)
		UTextureRenderTarget2D* windBuffer;

	UPROPERTY(EditAnywhere)
		UMaterialInstance* windUpdateMaterial;
	UPROPERTY()
		UMaterialInstanceDynamic* windUpdateDynamic;

	UPROPERTY(EditAnywhere)
		UMaterialInstance* windCopyToBufferMaterial;
	UPROPERTY()
		UMaterialInstanceDynamic* windCopyToBufferDynamic;

public:
	UFUNCTION(BlueprintPure)
		AChunkManager* GetChunkManager() const { return chunkManager; }

	UFUNCTION(BlueprintPure)
		UTextureRenderTarget2D* GetWindField() const { return windField; }
	UFUNCTION(BlueprintPure)
		UTextureRenderTarget2D* GetWindBuffer() const { return windBuffer; }

	UFUNCTION(BlueprintCallable)
		void SetWindOrigin(FVector Location, float Size);
};
