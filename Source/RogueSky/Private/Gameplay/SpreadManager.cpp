// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/SpreadManager.h"

// Sets default values
ASpreadManager::ASpreadManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	instancedStaticMesh = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>("Instanced Mesh");
	instancedStaticMesh->bDisableCollision = true;
}

// Called when the game starts or when spawned
void ASpreadManager::BeginPlay()
{
	Super::BeginPlay();
	instancedStaticMesh->SetWorldLocation(FVector::ZeroVector);
}

int ASpreadManager::CreateSpreadPoint() {
	spreadPoints.Add(false);
	return spreadPoints.Num() - 1;
}

bool ASpreadManager::ActivateSpreadPoint(int Index, FTransform Transform) {
	if (Index >= spreadPoints.Num() || Index < 0)
		return false;

	// Return if the spread point is already activated
	if (spreadPoints[Index] == true)
		return false;

	instancedStaticMesh->AddInstance(Transform, true);
	spreadPoints[Index] = true;
	activatedSpreadPoints++;
	return true;
}

