// Fill out your copyright notice in the Description page of Project Settings.
#include "Gameplay/TargeterComponent.h"

UTargeterComponent::UTargeterComponent() {
    PrimaryComponentTick.bCanEverTick = true;
}

void UTargeterComponent::SetTarget(USceneComponent* NewTarget) { 
    target = NewTarget; 
    if (target == nullptr)
        OnLoseTarget.Broadcast();
}

void UTargeterComponent::SetTargetLost() {
    target = nullptr;
    OnLoseTarget.Broadcast();
}

void UTargeterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!target.IsValid() && target != nullptr)
		SetTargetLost();

    if (target.IsValid())
        vectorToTarget = (target->GetComponentLocation() - GetOwner()->GetActorLocation()).GetSafeNormal();
}