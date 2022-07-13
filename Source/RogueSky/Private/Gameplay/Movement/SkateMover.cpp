// Fill out your copyright notice in the Description page of Project Settings.
#include "Gameplay/Movement/SkateMover.h"
#include "Kismet/KismetMathLibrary.h"

USkateMover::USkateMover() {
    priority = 0;
    bQueueable = true;
    PrimaryComponentTick.bCanEverTick = true;
}

void USkateMover::DoGroundMovement_Implementation(float DeltaTime, FVector DesiredMovement) {
    direction = UKismetMathLibrary::VInterpTo(direction.GetSafeNormal(), DesiredMovement.GetSafeNormal(), DeltaTime, rotationSpeed);

    movementComponent->SetGravity(0.0f);
    skateSpeed += GetSlopeBoost(movementComponent->GetGroundTrace()) * DeltaTime * 120.0f;
    movementComponent->SetPlanarVelocity(direction * skateSpeed);
    GetOwner()->SetActorRotation(direction.ToOrientationRotator());
}

void USkateMover::DoAerialMovement_Implementation(float DeltaTime, FVector DesiredMovement) {
    direction = UKismetMathLibrary::VInterpTo(direction.GetSafeNormal(), DesiredMovement.GetSafeNormal(), DeltaTime, rotationSpeed);
    movementComponent->SetPlanarVelocity(direction * skateSpeed);
    GetOwner()->SetActorRotation(direction.ToOrientationRotator());
}

float USkateMover::GetSlopeBoost(FHitResult GroundTrace) const {
    float boost = 0.0f;

    if (GroundTrace.IsValidBlockingHit()) {
        float slopePitch;
        float slopeRoll;
        UKismetMathLibrary::GetSlopeDegreeAngles(GetOwner()->GetActorRightVector(), GroundTrace.Normal, FVector::UpVector, slopePitch, slopeRoll);
        boost = (-slopePitch * downSlopeAcceleration) * (skateSpeed / maxSkateSpeed);

        if (boost > 0.0f) {
            float boostMult = (maxSkateSpeed - FMath::Min(skateSpeed, maxSkateSpeed)) / maxSkateSpeed;
            boostMult = FMath::Max(boostMult, 0.05f);
            boost *= boostMult;
        }
    }
    return boost;
}

void USkateMover::OnActivate_Implementation(FVector DesiredMovement) {
    skateSpeed = movementComponent->GetPlanarVelocity().Size();
    direction = movementComponent->GetPlanarVelocity().GetSafeNormal();
    movementComponent->bFriction = false;
}

void USkateMover::OnDeactivate_Implementation() {
    skateSpeed = 0.0f;
    movementComponent->bFriction = true;
}
