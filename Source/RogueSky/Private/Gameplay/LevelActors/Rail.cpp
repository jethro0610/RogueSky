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
}

void ARail::BeginPlay() {
    FVector currentLocation = sceneComponent->GetComponentLocation();
    float inputKey = spline->FindInputKeyClosestToWorldLocation(currentLocation);
    while (inputKey < spline->GetNumberOfSplinePoints() - 1.0f) { // Loop until end of spline is reached
        // Step forward based on the length of the collision section
        FVector splineLocation = spline->GetLocationAtSplineInputKey(inputKey, ESplineCoordinateSpace::World);
        FVector splineTangent = spline->GetTangentAtSplineInputKey(inputKey, ESplineCoordinateSpace::World).GetSafeNormal();
        FVector deltaVector = splineTangent * colliderSectionLength;
        currentLocation = splineLocation + deltaVector;

        // Create the new collider
        UCapsuleComponent* newCollider = NewObject<UCapsuleComponent>(this);
        newCollider->SetCapsuleHalfHeight(colliderSectionLength + 15.0f);
        newCollider->SetCapsuleRadius(railRadius);
        newCollider->SetWorldLocation(currentLocation);
        newCollider->SetWorldRotation(splineTangent.Rotation() + FRotator(90.0f, 0.0f, 0.0f));
        newCollider->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
        newCollider->SetHiddenInGame(false);
        newCollider->SetVisibility(true);
        newCollider->RegisterComponent();

        // Step forward for placement of next collider and get the new location on the spline
        currentLocation += deltaVector;
        inputKey = spline->FindInputKeyClosestToWorldLocation(currentLocation);
    }
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

