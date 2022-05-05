// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/LevelActors/Rail.h"

// Sets default values
ARail::ARail() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    spline = CreateDefaultSubobject<USplineComponent>("Spline");
    bounds = CreateDefaultSubobject<UBoxComponent>("Bounds");
}

void ARail::LocationIsTouchingRail(FVector Location, bool& IsTouching, FVector& LocationOnRail) const {
    IsTouching = false;
    FVector splineLocation = spline->FindLocationClosestToWorldLocation(Location, ESplineCoordinateSpace::World);
    if (FVector::Dist(splineLocation, Location) <= railRadius) {
        IsTouching = true;
        LocationOnRail = splineLocation;
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

