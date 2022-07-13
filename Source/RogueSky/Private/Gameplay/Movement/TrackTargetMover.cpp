// Fill out your copyright notice in the Description page of Project Settings.
#include "Gameplay/Movement/TrackTargetMover.h"
#include "Kismet/KismetMathLibrary.h"

UTrackTargetMover::UTrackTargetMover() {
    priority = 1;
}

void UTrackTargetMover::BeginPlay() {
    Super::BeginPlay();
    targeterComponent = GetOwner()->FindComponentByClass<UTargeterComponent>();
}

void UTrackTargetMover::DoMovement_Implementation(float DeltaTime, FVector DesiredMovement) {
    FVector targetVector = targeterComponent == nullptr ? GetOwner()->GetActorForwardVector() : targeterComponent->GetVectorToTarget();
    if(!bTrackZ)
        targetVector = FVector(targetVector.X, targetVector.Y, 0.0f);

    GetOwner()->SetActorRotation(targetVector.GetSafeNormal().ToOrientationRotator());

    float velocitySize = bTrackZ ? movementComponent->GetVelocity().Size() : movementComponent->GetPlanarVelocity().Size();
    if (desiredSpeed != 0.0f)
        velocitySize = UKismetMathLibrary::FInterpTo(velocitySize, desiredSpeed, DeltaTime, lerpSpeed);

    FVector forwardVector;
    if (!bTrackZ) {
        forwardVector = UKismetMathLibrary::RInterpTo(movementComponent->GetPlanarVelocity().ToOrientationRotator(), GetOwner()->GetActorForwardVector().ToOrientationRotator(), DeltaTime, trackingSpeed).Vector();
        movementComponent->SetPlanarVelocity(forwardVector * velocitySize);
    }
    else {
        forwardVector = UKismetMathLibrary::RInterpTo(movementComponent->GetVelocity().ToOrientationRotator(), GetOwner()->GetActorForwardVector().ToOrientationRotator(), DeltaTime, trackingSpeed).Vector();
        movementComponent->SetVelocity(forwardVector * velocitySize);
    }
}

void UTrackTargetMover::OnActivate_Implementation(FVector DesiredMovement) {
    movementComponent->bFriction = false;
}

void UTrackTargetMover::OnDeactivate_Implementation() {
    movementComponent->bFriction = true;
}

void UTrackTargetMover::StopTracking() {
    if (IsActiveMover())
        movementComponent->UseQueuedMover();
}