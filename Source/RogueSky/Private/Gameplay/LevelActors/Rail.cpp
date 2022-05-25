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

void ARail::LocationIsTouchingRail(FVector Location, bool& IsTouching, FVector& LocationOnRail) const {
    IsTouching = false;
    LocationOnRail = spline->FindLocationClosestToWorldLocation(Location, ESplineCoordinateSpace::World);
    if (FVector::Dist(LocationOnRail, Location) <= railRadius) {
        IsTouching = true;
    }
}

FTransform ARail::GetTransformAlongRail(FVector Location, FVector Velocity) const {
    FTransform splineTransform = spline->FindTransformClosestToWorldLocation(Location, ESplineCoordinateSpace::World);
    FVector splineTangent = splineTransform.GetRotation().Vector();

    FVector deltaLocation;
    if (FVector::DotProduct(Velocity.GetSafeNormal(), splineTangent) >= 0)
        deltaLocation = splineTangent * Velocity.Size();
    else
        deltaLocation = splineTangent * -Velocity.Size();

    return spline->FindTransformClosestToWorldLocation(splineTransform.GetLocation() + deltaLocation, ESplineCoordinateSpace::World);
}

