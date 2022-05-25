// Fill out your copyright notice in the Description page of Project Settings.
#include "Gameplay/Movers/RailMover.h"
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

    if (movementComponent->GetActiveMover() == this)
        return;

    for (ARail* railToCheck : railsInBounds) {
        bool isTouching;
        FVector nearestPointOnRail;
        railToCheck->LocationIsTouchingRail(GetOwner()->GetActorLocation(), isTouching, nearestPointOnRail);

        if (isTouching) {
            StartRailMovement(railToCheck, nearestPointOnRail);
        }
    }
}

void URailMover::StartRailMovement(ARail* RailToMoveOn, FVector MoveStartLocation) {
    rail = RailToMoveOn;
    GetOwner()->SetActorLocation(MoveStartLocation);
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
