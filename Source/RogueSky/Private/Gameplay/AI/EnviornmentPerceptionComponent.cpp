// Fill out your copyright notice in the Description page of Project Settings.
#include "Gameplay/AI/EnviornmentPerceptionComponent.h"
#include "Kismet/KismetMathLibrary.h"

UEnviornmentPerciever::UEnviornmentPerciever() {
    PrimaryComponentTick.bCanEverTick = true;
    SetCollisionResponseToAllChannels(ECR_Ignore);
    SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    SetVisibility(false);
    SphereRadius = 2500.0f;
}

void UEnviornmentPerciever::BeginPlay() {
    Super::BeginPlay();
    OnComponentBeginOverlap.AddDynamic(this, &UEnviornmentPerciever::OnBeginOverlap);
    OnComponentEndOverlap.AddDynamic(this, &UEnviornmentPerciever::OnEndOverlap);
}

void UEnviornmentPerciever::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    for (TWeakObjectPtr<AActor> percievedActor : actorsInPerceptionDistance) {
        FVector vectorToActor = (percievedActor->GetActorLocation() - GetOwner()->GetActorLocation()).GetSafeNormal();
        float perceptionDotProduct = FVector::DotProduct(GetOwner()->GetActorForwardVector(), vectorToActor);
        float perceptionAngle = FMath::Abs(FMath::RadiansToDegrees(FMath::Acos(perceptionDotProduct)));
        if (perceptionAngle <= maxPerceptionAngle) {
            if (!percievedActors.Contains(percievedActor)) {
                percievedActors.Add(percievedActor);
                OnPercievePlayer.Broadcast(percievedActor.Get());
            }
        }
        else {
            percievedActors.Remove(percievedActor);
        }
    }
}

void UEnviornmentPerciever::OnBeginOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult) {
    if (OtherActor->ActorHasTag(TEXT("Player")))
        actorsInPerceptionDistance.Add(OtherActor);
}

void UEnviornmentPerciever::OnEndOverlap(class UPrimitiveComponent* OverlapComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) {
    actorsInPerceptionDistance.Remove(OtherActor);
    percievedActors.Remove(OtherActor);
}