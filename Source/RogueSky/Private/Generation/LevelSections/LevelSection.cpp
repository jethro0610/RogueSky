// Fill out your copyright notice in the Description page of Project Settings.
#include "Generation/LevelSections/LevelSection.h"

// Sets default values
ALevelSection::ALevelSection() {
	sectionArea = CreateDefaultSubobject<USphereComponent>("Section Range");
	sectionArea->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	sectionArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetRootComponent(sectionArea);

	instancedStaticMesh = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>("Instanced Mesh");
	instancedStaticMesh->bDisableCollision = true;
}

// Called when the game starts or when spawned
void ALevelSection::BeginPlay() {
	Super::BeginPlay();
	sectionArea->SetSphereRadius(FMath::RandRange(minRadius, maxRadius));
}

void ALevelSection::AddSpreadPoint(int X, int Y, int Z) {
	spreadPoints.Add(GetLocationTuple(X, Y, Z), false);
}

bool ALevelSection::ActivateSpreadPoint(int X, int Y, int Z) {
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

TTuple<int16, int16, int16> ALevelSection::GetLocationTuple(int16 X, int16 Y, int16 Z) const {
	return TTuple<int16, int16, int16>(X, Y, Z);
}
