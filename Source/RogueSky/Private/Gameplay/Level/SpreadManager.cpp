// Fill out your copyright notice in the Description page of Project Settings.
#include "Gameplay/Level/SpreadManager.h"

// Sets default values
ASpreadManager::ASpreadManager() {
	instancedStaticMesh = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>("Instanced Mesh");
	instancedStaticMesh->bDisableCollision = true;
	SetRootComponent(instancedStaticMesh);
}

// Called when the game starts or when spawned 
void ASpreadManager::BeginPlay() {
	Super::BeginPlay();
}

void ASpreadManager::AddSpreadPoint(int X, int Y, int Z) {
	spreadPoints.Add(GetLocationTuple(X, Y, Z), false);
}

bool ASpreadManager::ActivateSpreadPoint(int X, int Y, int Z) {
	auto locationTuple = GetLocationTuple(X, Y, Z);
	if (!spreadPoints.Contains(locationTuple))
		return false;

	// Skip if the spread point was already activated
	if (spreadPoints[locationTuple] == true)
		return false;

	spreadPoints[locationTuple] = true;
	activatedSpreadCount++;
	return true;
}

TTuple<uint16, uint16, uint16> ASpreadManager::GetLocationTuple(uint16 X, uint16 Y, uint16 Z) {
	return TTuple<uint16, uint16, uint16>(X, Y, Z);
}

