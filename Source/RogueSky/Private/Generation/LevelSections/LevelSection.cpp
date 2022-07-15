// Fill out your copyright notice in the Description page of Project Settings.
#include "Generation/LevelSections/LevelSection.h"

// Sets default values
ALevelSection::ALevelSection() {
	sectionArea = CreateDefaultSubobject<USphereComponent>("Section Range");
	sectionArea->SetCollisionProfileName("LevelSection");
	SetRootComponent(sectionArea);

	instancedStaticMesh = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>("Instanced Mesh");
	instancedStaticMesh->bDisableCollision = true;
}

// Called when the game starts or when spawned
void ALevelSection::BeginPlay() {
	Super::BeginPlay();
	sectionArea->SetSphereRadius(FMath::RandRange(minRadius, maxRadius));
}

void ALevelSection::CreateSpreadPoints() {
	for (float x = -GetRadius(); x < GetRadius(); x += spreadSpacing)
	for (float y = -GetRadius(); y < GetRadius(); y += spreadSpacing) {
		if (!LocationIsInside(FVector2D(x, y) + GetLocation2D(), 1024.0f))
			continue;

		int16 intX = x / spreadSpacing;
		int16 intY = y / spreadSpacing;
		spreadPoints.Add(GetLocationTuple(intX, intY), false);
	}
}

bool ALevelSection::ActivateSpreadPoint(int X, int Y) {
	auto locationTuple = GetLocationTuple(X, Y);
	if (!spreadPoints.Contains(locationTuple))
		return false;

	// Skip if the spread point was already activated
	if (spreadPoints[locationTuple] == true)
		return false;

	spreadPoints[locationTuple] = true;
	activatedSpreadCount++;

	FTransform instanceTransform;
	FVector2D pointOrigin = GetLocation2D() + FVector2D(X, Y) * spreadSpacing;
	instanceTransform.SetLocation(GetLocationOnSurface(pointOrigin));
	instancedStaticMesh->AddInstance(instanceTransform);

	return true;
}

TTuple<int16, int16> ALevelSection::GetLocationTuple(int16 X, int16 Y) const {
	return TTuple<int16, int16>(X, Y);
}