// Fill out your copyright notice in the Description page of Project Settings.
#include "Gameplay/Movers/RailMover.h"
#include "Kismet/KismetMathLibrary.h"
#include "Gameplay/LevelActors/Rail.h"

URailMover::URailMover() {
    PrimaryComponentTick.bCanEverTick = true;
    priority = 1;
}

void URailMover::BeginPlay() {
    Super::BeginPlay();

    UCapsuleComponent* collider = GetOwner()->FindComponentByClass<UCapsuleComponent>();
    collider->OnComponentBeginOverlap.AddDynamic(this, &URailMover::OnOverlap);
    collider->OnComponentEndOverlap.AddDynamic(this, &URailMover::EndOverlap);
}

void URailMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    // Don't check for collision till cooldown is over
    if (cooldown >= 0.0f) {
        cooldown -= DeltaTime;
        return;
    }

    if (movementComponent->GetActiveMover() == this)
        return;

    for (ARail* railToCheck : railsInBounds) {
        bool isTouching;
        FTransform transformOnRail;
        railToCheck->LocationIsTouchingRail(GetOwner()->GetActorLocation(), isTouching, transformOnRail);

        if (isTouching) {
            StartRailMovement(railToCheck, transformOnRail);
        }
    }
}

void URailMover::StartRailMovement(ARail* RailToMoveOn, FTransform RailTransform) {
    rail = RailToMoveOn;
    railTransform = RailTransform;
    GetOwner()->SetActorLocation(railTransform.GetLocation());

    FVector flatRailTangent = railTransform.GetRotation().Vector();
    flatRailTangent = FVector(flatRailTangent.X, flatRailTangent.Y, 0.0f).GetSafeNormal();

    float forwardDot = FVector::DotProduct(movementComponent->GetPlanarVelocity().GetSafeNormal(), flatRailTangent);
    currentSpeed = movementComponent->GetPlanarVelocity().Size() * forwardDot;
    movementComponent->SetActiveMover(this);

}

void URailMover::OnActivate_Implementation(FVector DesiredMovement) {
    movementComponent->bFriction = false;
    movementComponent->bGravity = false;
    movementComponent->SetVelocity(FVector::ZeroVector);
}

void URailMover::OnDeactivate_Implementation() {
    movementComponent->bFriction = true;
    movementComponent->bGravity = true;
    rail = nullptr;
    cooldown = cooldownLength;
}

void URailMover::DoMovement_Implementation(float DeltaTime, FVector DesiredMovement) {
    FVector flatRailTangent = railTransform.GetRotation().Vector();
    flatRailTangent = FVector(flatRailTangent.X, flatRailTangent.Y, 0.0f).GetSafeNormal();

    if (DesiredMovement.Size() > 0.1f) {
        float forwardDot = FVector::DotProduct(DesiredMovement.GetSafeNormal(), flatRailTangent);
        float speedToUse;

        if (forwardDot * currentSpeed >= 0.0f) // Determine if the desired direction and the current travel direction are the same (would create a positive number since matching sign)
            speedToUse = accelerationSpeed; // Calculate acceleration  if they are the same
        else
            speedToUse = breakSpeed; // Calculate breaking if they are opposites


        currentSpeed = UKismetMathLibrary::FInterpTo(currentSpeed, maxSpeed * forwardDot, DeltaTime, speedToUse);
    }

    movementComponent->SetVelocity(railTransform.GetRotation().Vector() * currentSpeed);
    railTransform = rail->GetTransformAlongRail(GetOwner()->GetActorLocation());
    GetOwner()->SetActorLocation(railTransform.GetLocation());

    FVector exitPoint;
    if (currentSpeed > 0.0f)
        exitPoint = rail->GetEndpoint();
    else
        exitPoint = rail->GetStartpoint();

    float distanceFromEnd = FVector::Dist(GetOwner()->GetActorLocation(), exitPoint);
    if (distanceFromEnd <= 50.0f) {
        movementComponent->UseQueuedMover();
    }
}

void URailMover::OnOverlap(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult) {
    
    ARail* overlappedRail = Cast<ARail>(OtherActor);
    if (overlappedRail != nullptr && !railsInBounds.Contains(overlappedRail)) {
        railsInBounds.Add(overlappedRail);
    }
}

void URailMover::EndOverlap(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex) {

    ARail* overlappedRail = Cast<ARail>(OtherActor);
    if (railsInBounds.Contains(overlappedRail))
        railsInBounds.Remove(overlappedRail);
}
