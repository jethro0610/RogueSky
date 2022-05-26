// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/LevelActors/Rail.h"

// Sets default values
ARail::ARail() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    sceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    SetRootComponent(sceneComponent);

    spline = CreateDefaultSubobject<USplineComponent>("Spline");
    spline->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);

    collider = CreateDefaultSubobject<UBoxComponent>("Collider");
    collider->AttachToComponent(spline, FAttachmentTransformRules::SnapToTargetIncludingScale);
}

void ARail::BeginPlay() {
    collider->SetBoxExtent(spline->GetLocalBounds().BoxExtent);
    collider->SetRelativeLocation(spline->GetLocalBounds().Origin);
}

void ARail::LocationIsTouchingRail(FVector Location, bool& IsTouching, FTransform& TransformOnRail) const {
    IsTouching = false;
    TransformOnRail = spline->FindTransformClosestToWorldLocation(Location, ESplineCoordinateSpace::World);
    if (FVector::Dist(TransformOnRail.GetLocation(), Location) <= railRadius) {
        IsTouching = true;
    }
}

FTransform ARail::GetTransformAlongRail(FVector Location) const {
    return spline->FindTransformClosestToWorldLocation(Location, ESplineCoordinateSpace::World);
}

FVector ARail::GetStartpoint() const {
    return spline->GetLocationAtSplineInputKey(0.0f, ESplineCoordinateSpace::World);
}

FVector ARail::GetEndpoint() const {
    return spline->GetLocationAtTime(spline->GetNumberOfSplinePoints() - 1.0f, ESplineCoordinateSpace::World);
}

