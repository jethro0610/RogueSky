// Fill out your copyright notice in the Description page of Project Settings.
#include "Generation/Placers/SpreadPlacer.h"

void USpreadPlacer::Initialize(UIslandGenerator* Generator) {
    generator = Generator;
    startPoint = FVector2D(generator->GetOrigin()) - FVector2D(generator->GetMaxRadius(), generator->GetMaxRadius());
    endPoint = FVector2D(generator->GetOrigin()) + FVector2D(generator->GetMaxRadius(), generator->GetMaxRadius());
    distanceBetweenPlacements = 150.0f;
}

void USpreadPlacer::Place() {
    for (int x = startPoint.X; x <= endPoint.X; x += distanceBetweenPlacements)
    for (int y = startPoint.Y; y <= endPoint.Y; y += distanceBetweenPlacements) {
        FVector2D spawnPoint(x, y);
        if (!generator->GetBlobMask().PointIsInBlob(spawnPoint, 2048.0f))
            continue;

        float randomAngle = FMath::Rand();
        FVector2D randomDireciton = FVector2D(FMath::Cos(randomAngle), FMath::Sin((float)randomAngle));
        float randomDistance = (FMath::Rand() / (float)RAND_MAX) * distanceBetweenPlacements * 0.5f;

        FVector spawnLocation = generator->GetLocationOnSurface(spawnPoint + randomDireciton * randomDistance);
        FRotator spawnRotation = FRotator(0.0f, randomAngle, 0.0f);
        GetWorld()->SpawnActor(ASpreadPoint::StaticClass(), &spawnLocation, &spawnRotation);
    }
}