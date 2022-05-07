// Fill out your copyright notice in the Description page of Project Settings.
#include "Gameplay/Movers/RailMover.h"
#include "Components/CapsuleComponent.h"

void URailMover::BeginPlay() {
    Super::BeginPlay();

    UCapsuleComponent* collider = GetOwner()->FindComponentByClass<UCapsuleComponent>();
    collider->OnComponentBeginOverlap.AddDynamic(this, &URailMover::OnOverlap);
    collider->OnComponentEndOverlap.AddDynamic(this, &URailMover::EndOverlap);
}

void URailMover::OnOverlap(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult) {
    
    ARail* overlappedRail = Cast<ARail>(OtherActor);
    if (overlappedRail != nullptr && !railsInBounds.Contains(overlappedRail))
        railsInBounds.Add(overlappedRail);
}

void URailMover::EndOverlap(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex) {

    ARail* overlappedRail = Cast<ARail>(OtherActor);
    if (railsInBounds.Contains(overlappedRail))
        railsInBounds.Remove(overlappedRail);
}
