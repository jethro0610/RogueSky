// Fill out your copyright notice in the Description page of Project Settings.
#include "Gameplay/LevelActors/Rail.h"
#include "Kismet/KismetMathLibrary.h"

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
    //UpdateBounds();
    //CreateMeshes();
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

void ARail::UpdateBounds() {
    collider->SetBoxExtent(spline->GetLocalBounds().BoxExtent);
    collider->SetRelativeLocation(spline->GetLocalBounds().Origin);
}

void ARail::CreateMeshes() {
    for (int i = 0; i < spline->GetNumberOfSplinePoints() - 1; i++) {
        USplineMeshComponent* newMesh = NewObject<USplineMeshComponent>(this);
        newMesh->SetStaticMesh(mesh);
        newMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

        FVector startVector = spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
        FVector startTangent = spline->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::World);
        FVector endVector = spline->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::World);
        FVector endTangent = spline->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::World);

        newMesh->SetStartAndEnd(startVector, startTangent, endVector, endTangent);
        newMesh->RegisterComponent();
    }
}

void ARail::AddPoint(FVector Location) {
    spline->AddSplinePoint(Location, ESplineCoordinateSpace::World, true);
}

void ARail::SetEndpointLocation(FVector Location) {
    spline->SetLocationAtSplinePoint(spline->GetNumberOfSplinePoints() - 1, Location, ESplineCoordinateSpace::World, true);
    //spline->AddSplinePointAtIndex((GetActorLocation() + Location) / 2.0f, 1, ESplineCoordinateSpace::World, true); // This adds a point in the middle
}

void ARail::ConnectIslands(IslandGenerator* Island1, IslandGenerator* Island2) {
    FVector2D locationFrom1to2 = (Island2->GetBlobMask().GetOrigin() - Island1->GetBlobMask().GetOrigin()).GetSafeNormal();
    float island1EdgeDistance = Island1->GetBlobMask().GetEdgeDistanceFromOrigin(locationFrom1to2) - 2000.0f;
    float island2EdgeDistance = Island2->GetBlobMask().GetEdgeDistanceFromOrigin(-locationFrom1to2) - 2000.0f;

    FVector2D island1Location2D = Island1->GetBlobMask().GetOrigin() + island1EdgeDistance * locationFrom1to2;
    FVector2D island2Location2D = Island2->GetBlobMask().GetOrigin() + island2EdgeDistance * -locationFrom1to2;

    FVector island1Location = Island1->GetLocationOnSurface(island1Location2D) + FVector(0.0f, 0.0f, 50.0f);
    FVector island2Location = Island2->GetLocationOnSurface(island2Location2D) + FVector(0.0f, 0.0f, 50.0f);;

    SetActorLocation(island1Location);
    SetEndpointLocation(island2Location);

    FVector midLocation1 = UKismetMathLibrary::VLerp(island1Location, island2Location, 0.7f);
    midLocation1 += FVector(FMath::RandRange(-800.0f, 800.0f), FMath::RandRange(-800.0f, 800.0f), FMath::RandRange(0.0f, 800.0f));
    FVector midLocation2 = UKismetMathLibrary::VLerp(island1Location, island2Location, 0.3f);
    midLocation2 += FVector(FMath::RandRange(-800.0f, 800.0f), FMath::RandRange(-800.0f, 800.0f), FMath::RandRange(0.0f, 800.0f));

    spline->AddSplinePointAtIndex(midLocation1, 1, ESplineCoordinateSpace::World, true);
    spline->AddSplinePointAtIndex(midLocation2, 1, ESplineCoordinateSpace::World, true);

    UpdateBounds();
    CreateMeshes();
}

