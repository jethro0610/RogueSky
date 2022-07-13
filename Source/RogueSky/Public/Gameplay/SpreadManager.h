// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "SpreadManager.generated.h"

UCLASS()
class ROGUESKY_API ASpreadManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpreadManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
		UHierarchicalInstancedStaticMeshComponent* instancedStaticMesh;
	
	float spreadSpacing;
	TMap<TTuple<uint16, uint16, uint16>, bool> spreadPoints;
	uint16 activatedSpreadCount = 0;

public:
	UFUNCTION(BlueprintCallable)
		void AddSpreadPoint(int X, int Y, int Z);
	UFUNCTION(BlueprintCallable)
		bool ActivateSpreadPoint(int X, int Y, int Z);
	TTuple<uint16, uint16, uint16> GetLocationTuple(uint16 X, uint16 Y, uint16 Z);
};
