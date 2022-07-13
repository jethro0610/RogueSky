// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chunk.h"
#include "Components/RuntimeMeshComponentStatic.h"
#include "ChunkManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDoneGenerating);

UCLASS()
class ROGUESKY_API AChunkManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunkManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void BeginDestroy() override;

private:
	Chunk* chunks[CHUNK_COUNT][CHUNK_COUNT][CHUNK_COUNT] = { nullptr };

	UPROPERTY()
		TArray<URuntimeMeshComponent*> meshes;

	UPROPERTY()
		TArray<URuntimeMeshComponent*> stitches;

	UPROPERTY(EditAnywhere)
		UMaterialInstance* material;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> actorToPlace;

	FVector randomLocation;

	bool bGenerated = false;

public:
	UPROPERTY(BlueprintAssignable)
		FOnDoneGenerating OnDoneGenerating;

	Chunk* GetChunk(int8 X, int8 Y, int8 Z) const;

	Chunk* GetChunk(FIntVector8 Location) const {
		return GetChunk(Location.X, Location.Y, Location.Z);
	}

	Chunk* CreateChunk(int8 X, int8 Y, int8 Z);

	UFUNCTION(BlueprintPure)
		bool IsGenerated() const { return bGenerated; }
};
