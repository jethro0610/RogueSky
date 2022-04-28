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
	inline static ASpreadManager* spreadManager;
	UPROPERTY(VisibleAnywhere)
		UHierarchicalInstancedStaticMeshComponent* instancedStaticMesh;
	
	TArray<bool> spreadIndexes;
	int activatedSpreadIndexes;

public:
	UFUNCTION(BlueprintCallable)
		int CreateSpreadIndex();
	UFUNCTION(BlueprintCallable)
		bool ActivateSpreadIndex(int Index, FTransform Transform);

	static ASpreadManager* GetSpreadManager() { return spreadManager; }
};
