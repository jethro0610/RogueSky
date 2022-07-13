// Fill out your copyright notice in the Description page of Project Settings.
#include "Gameplay/Movers/GroundMover.h"
#include "Gameplay/VelocityMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UGroundMover::UGroundMover() {
	priority = 0;
	bQueueable = true;
}

void UGroundMover::BeginPlay() {
	Super::BeginPlay();
	defaultMaxSpeed = maxSpeed;
	defaultFriction = movementComponent->groundFriction;
}

void UGroundMover::OnActivate_Implementation(FVector DesiredMovement) {
	if (bTransferMomentum)
		maxSpeed = movementComponent->GetPlanarVelocity().Size();
}

void UGroundMover::DoGroundMovement_Implementation(float DeltaTime, FVector DesiredMovement) {
	if (bTransferMomentum) {
		maxSpeed -= momentumDecay * DeltaTime * 120.0f;
		float dotProductDiff = 1.0f - movementComponent->GetPlanarDotProduct();
		maxSpeed -= dotProductDiff * 100.0f * momentumCancelStrength * DeltaTime * 120.0f;
		maxSpeed = FMath::Max(maxSpeed, defaultMaxSpeed);

		float frictionLerp = (maxSpeed - defaultMaxSpeed) / (peakSpeedForFriction - defaultMaxSpeed);
		frictionLerp = 1.0f - FMath::Clamp(frictionLerp, 0.0f, 1.0f);
		frictionLerp = frictionLerp * frictionLerp * frictionLerp;
		frictionLerp = 1.0f - frictionLerp;
		movementComponent->groundFriction = FMath::Lerp(defaultFriction, minimumMomentumFriction, frictionLerp);
	}

	if (DesiredMovement == FVector::ZeroVector)
		return;

	FRotator lerpRotation = UKismetMathLibrary::RInterpTo(GetOwner()->GetActorRotation(), DesiredMovement.ToOrientationRotator(), DeltaTime, rotationSpeed);
	GetOwner()->SetActorRotation(lerpRotation);

	if (bUseForwardWalk)
		DesiredMovement = GetOwner()->GetActorForwardVector() * DesiredMovement.Size();

	float groundAcceleration = (maxSpeed * movementComponent->groundFriction) / (-movementComponent->groundFriction + 1.0f);
	movementComponent->AddVelocity(groundAcceleration * DesiredMovement * DeltaTime * 120.0f);
}

void UGroundMover::DoAerialMovement_Implementation(float DeltaTime, FVector DesiredMovement) {
	if (DesiredMovement == FVector::ZeroVector)
		return;

	float airAcceleration = (maxSpeed * movementComponent->airFriction) / (-movementComponent->airFriction + 1.0f);
	movementComponent->AddVelocity(airAcceleration * DesiredMovement * DeltaTime * 120.0f);
	FRotator lerpRotation = UKismetMathLibrary::RInterpTo(GetOwner()->GetActorRotation(), DesiredMovement.ToOrientationRotator(), DeltaTime, 12.0f);
	GetOwner()->SetActorRotation(lerpRotation);
}
