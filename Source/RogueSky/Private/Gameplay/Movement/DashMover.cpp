// Fill out your copyright notice in the Description page of Project Settings.
#include "Gameplay/Movement/DashMover.h"
#include "Kismet/KismetMathLibrary.h"

UDashMover::UDashMover() {
    priority = 1;
}

void UDashMover::OnActivate_Implementation(FVector DesiredMovement) {
    dashSpeed = FMath::Max(minDashSpeed, movementComponent->GetPlanarVelocity().Size());
    dashTimer = 0.0f;

    movementComponent->SetGravity(0.0f);
    movementComponent->bGravity = false;
    
    if (!DesiredMovement.IsNearlyZero()) {
        FRotator lerpRotation = UKismetMathLibrary::RLerp(
            movementComponent->GetPlanarVelocity().ToOrientationRotator(),
            DesiredMovement.ToOrientationRotator(),
            initialRotationInfluence, true);
        GetOwner()->SetActorRotation(lerpRotation);
    }
    else {
        GetOwner()->SetActorRotation(movementComponent->GetPlanarVelocity().ToOrientationRotator());
    }
}

void UDashMover::OnDeactivate_Implementation() {
    movementComponent->bGravity = true;
}

void UDashMover::DoMovement_Implementation(float DeltaTime, FVector DesiredMovement) {
    dashTimer += DeltaTime;

    float easeDashSpeed = FMath::Pow(FMath::Min(dashTimer / dashLength, 1.0f), easeInStrength);

    float dashVelocity = FMath::Lerp(
        dashSpeed * dashSpeedMultiplier,
        dashSpeed + exitSpeed,
        easeDashSpeed
    );

    if (!DesiredMovement.IsNearlyZero()) {
        FRotator lerpRotation = UKismetMathLibrary::RInterpTo(
            GetOwner()->GetActorRotation(), 
            DesiredMovement.ToOrientationRotator(),
            DeltaTime, 
            rotationSpeed);
        GetOwner()->SetActorRotation(lerpRotation);
    }
    movementComponent->SetPlanarVelocity(GetOwner()->GetActorForwardVector() * dashVelocity);

    if (dashTimer >= dashLength) {
        OnEndDash.Broadcast();
        movementComponent->UseQueuedMover();
    }
}

