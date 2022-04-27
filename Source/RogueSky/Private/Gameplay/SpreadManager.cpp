// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/SpreadManager.h"

// Sets default values
ASpreadManager::ASpreadManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	instancedStaticMesh = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>("INSTANCED MESH");
	instancedStaticMesh->bDisableCollision = true;
}

// Called when the game starts or when spawned
void ASpreadManager::BeginPlay()
{
	Super::BeginPlay();
	instancedStaticMesh->SetWorldLocation(FVector::ZeroVector);
}

void ASpreadManager::SpawnInstance(FVector Location) {
	instancedStaticMesh->AddInstance(FTransform(Location), true);
}

