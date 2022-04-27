// Fill out your copyright notice in the Description page of Project Settings.
#include "Generation/Placers/Placer.h"

Placer::Placer(TSubclassOf<AActor> ActorToPlace) {
    actorToPlace = ActorToPlace;
}

Placer::~Placer() {
}

void Placer::Place(IslandGenerator& Generator, UWorld* World) {
    BlobMask blobMask = Generator.GetBlobMask();

    int startAngleToEdge = FMath::Rand();
    FVector2D startEdgeDirection = FVector2D(FMath::Cos(startAngleToEdge), FMath::Sin((float)startAngleToEdge));
    float startEdgeDistance = blobMask.GetEdgeDistanceFromOrigin(startEdgeDirection);

    FVector2D spawnPoint = blobMask.GetOrigin() + startEdgeDirection * (startEdgeDistance - 2500.0f);
    FVector2D placementDirection = -startEdgeDirection;

    bool isInGenerator = true;
    while (isInGenerator) {
        float rotationAngle = ((FMath::Rand() % ((int)maxRotationAmount * 200)) / 100.0f) - maxRotationAmount;
        int placements = (FMath::Rand() % (maxPlacementsBeforeRotate - minPlacementsBeforeRotate)) + minPlacementsBeforeRotate;

        for (int i = 0; i < placements; i++) {
            FVector spawnLocation = Generator.GetLocationOnSurface(spawnPoint);

            World->SpawnActor(actorToPlace.Get(), &spawnLocation);

            spawnPoint += placementDirection * placementDistance;
            placementDirection = placementDirection.GetRotated(rotationAngle);

            if (!blobMask.PointIsInBlob(spawnPoint, 2500.0f)) {
                isInGenerator = false;
                break;
            }
        }
    }
}

void Placer::PlaceRandom(IslandGenerator& Generator, UWorld* World) {
    for (int i = 0; i < 8; i++) {
        FVector spawnLocation = Generator.GetRandomLocationOnSurface(1000.0f);
        World->SpawnActor(actorToPlace.Get(), &spawnLocation);
    }
}
