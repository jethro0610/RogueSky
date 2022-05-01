// Fill out your copyright notice in the Description page of Project Settings.
#include "Gameplay/Movers/TurnMover.h"

UTurnMover::UTurnMover() {
    priority = 0;
    bQueueable = true;
}

void UTurnMover::DoGroundMovement_Implementation(float DeltaTime, FVector DesiredMovement) {
    FRotator currentRotation = GetOwner()->GetActorRotation();
    float turnAmount = (rightTurnAmount - leftTurnAmount);
    GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, FString::SanitizeFloat(turnAmount));
    GetOwner()->SetActorRotation(currentRotation + FRotator(0.0f, turnAmount * turnSpeed * DeltaTime * 120.0f, 0.0f));
    movementComponent->SetVelocity(GetOwner()->GetActorForwardVector() * movementComponent->GetVelocity().Size());
}

void UTurnMover::OnActivate_Implementation(FVector DesiredMovement) {
    movementComponent->bFriction = false;
}

void UTurnMover::OnDeactivate_Implementation() {
    movementComponent->bFriction = true;
}