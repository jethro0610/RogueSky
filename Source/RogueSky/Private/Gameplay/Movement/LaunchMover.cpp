// Fill out your copyright notice in the Description page of Project Settings.
#include "Gameplay/Movers/LaunchMover.h"
#include "Kismet/KismetMathLibrary.h"

ULaunchMover::ULaunchMover() {
    priority = 2;
}

void ULaunchMover::BeginPlay() {
    Super::BeginPlay();
    collisionParams.AddIgnoredActor(GetOwner());
    targeterComponent = GetOwner()->FindComponentByClass<UTargeterComponent>();
    if (targeterComponent != nullptr)
        targeterComponent->OnLoseTarget.AddDynamic(this, &ULaunchMover::OnLoseTarget);
}

void ULaunchMover::DoMovement_Implementation(float DeltaTime, FVector DesiredMovement) {
    if (movementComponent->GetPlanarVelocity().IsNearlyZero())
        return;

    if (trackingSpeed > 0.0f && hasTarget) {
        // Decide whether or not to track the target. Distance is determined by Z positions
        // default value infinity is used when there is no tracking distance
        float distance = INFINITY;
        if (distanceToStopTracking != 0.0f && movementComponent->GetGravity() > 0.0f)
            distance = FMath::Abs(GetOwner()->GetActorLocation().Z - targeterComponent->GetTarget()->GetComponentLocation().Z);

        if (distance > distanceToStopTracking) {
            FVector targetVelocity = GetPlanarVelocityToTarget();
            FVector lerpVelocity = UKismetMathLibrary::VInterpTo(movementComponent->GetPlanarVelocity(), targetVelocity, DeltaTime, trackingSpeed);
            movementComponent->SetPlanarVelocity(lerpVelocity);
        }
    }

    roll += rollSpeed * DeltaTime * 120.0f;
    FRotator rotation = movementComponent->GetVelocity().ToOrientationRotator();
    rotation = FRotator(rotation.Pitch - 90.0f, rotation.Yaw, 0.0f);
    GetOwner()->SetActorRotation(rotation);
    GetOwner()->AddActorLocalRotation(FRotator(0.0f, roll, 0.0f));

    FHitResult headTrace;
    FVector endTraceLocation = GetOwner()->GetActorLocation() + GetOwner()->GetActorUpVector() * headTraceDistance;
    GetWorld()->LineTraceSingleByChannel(headTrace, GetOwner()->GetActorLocation(), endTraceLocation, ECollisionChannel::ECC_WorldDynamic, collisionParams);
    if (headTrace.IsValidBlockingHit()) {
        FVector actorLocation = headTrace.Location - GetOwner()->GetActorUpVector() * headTraceDistance;
        GetOwner()->SetActorLocation(actorLocation);
        movementComponent->SetMovementEnabled(false);
        OnEndLaunch.Broadcast();
    }
}

void ULaunchMover::OnActivate_Implementation(FVector DesiredMovement) {
    if (targeterComponent == nullptr)
        return;

    if (targeterComponent->GetTarget() == nullptr)
        return;

    hasTarget = true;
    roll = 0.0f;
    movementComponent->gravitySpeed *= gravityMultiplier;
    movementComponent->bFriction = false;
    movementComponent->bStickToGround = false;
    movementComponent->SetGravity(-verticalLaunchVelocity);
    movementComponent->SetPlanarVelocity(GetPlanarVelocityToTarget());
    OnStartLaunch.Broadcast();
}

void ULaunchMover::OnDeactivate_Implementation() {
    movementComponent->gravitySpeed /= gravityMultiplier;
    movementComponent->bFriction = true;
    movementComponent->bGravity = true;
    movementComponent->bStickToGround = true;
}

FVector ULaunchMover::GetPlanarVelocityToTarget() {
    // Get variables for quadratic formula
    float a = -movementComponent->gravitySpeed / 2.0f;
    float b = movementComponent->GetVelocity().Z;
    float bSquared = b * b;
    float c = GetOwner()->GetActorLocation().Z - targeterComponent->GetTarget()->GetComponentLocation().Z;

    // Quadratic formula for air time
    float airTime = -b - FMath::Sqrt(bSquared - 4 * a * c);
    airTime /= 2 * a;

    FVector planarLocation = GetOwner()->GetActorLocation();
    planarLocation = FVector(planarLocation.X, planarLocation.Y, 0.0f);
    FVector targetPlanarLocation = targeterComponent->GetTarget()->GetComponentLocation();
    targetPlanarLocation = FVector(targetPlanarLocation.X, targetPlanarLocation.Y, 0.0f);
    FVector direction = (targetPlanarLocation - planarLocation).GetSafeNormal();
    float distance = FVector::Dist(planarLocation, targetPlanarLocation);

    float horizontalSpeed = distance / airTime;
    return direction * horizontalSpeed;
}
