// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Generation/Chunk/ChunkManager.h"
#include "Gameplay/Level/SpreadManager.h"
#include "GameFramework/GameModeBase.h"
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

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	AChunkManager* chunkManager;
	ASpreadManager* spreadManager;

public:
	UFUNCTION(BlueprintPure)
		AChunkManager* GetChunkManager() const { return chunkManager; }
	UFUNCTION(BlueprintPure)
		ASpreadManager* GetSpreadManager() const { return spreadManager; }
};
