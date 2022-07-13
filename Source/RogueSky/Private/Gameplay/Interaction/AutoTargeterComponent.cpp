// Fill out your copyright notice in the Description page of Project Settings.
#include "Gameplay/Interaction/AutoTargeterComponent.h"

UAutoTargeterComponent::UAutoTargeterComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UAutoTargeterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (target != nullptr) {
		if (FVector::Dist(GetOwner()->GetActorLocation(), target->GetComponentLocation()) > targetRadius) {
			SetTargetLost();
		}
	}

	if (!bLockOn)
		SetTargetToNearest();
}

void UAutoTargeterComponent::SetTargetLost() {
	UTargeterComponent::SetTargetLost();
	bLockOn = false;
}

FVector UAutoTargeterComponent::GetVectorToTarget() const {
	if (!target.IsValid())
		return GetOwner()->GetActorForwardVector();

	return UTargeterComponent::GetVectorToTarget();
}

USceneComponent* UAutoTargeterComponent::FindNearestTarget(float Radius, bool bMustBeRendered) const {
	TArray<FOverlapResult> overlaps;
	FCollisionShape shape;
	FCollisionQueryParams params;
	params.AddIgnoredActor(GetOwner());
	shape.SetSphere(Radius);
	GetWorld()->OverlapMultiByChannel(overlaps, GetOwner()->GetActorLocation(), FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1, shape, params);

	USceneComponent* returnTarget = nullptr;
	float shortestDistance = INFINITY;
	for (FOverlapResult overlap : overlaps) {
		if (bMustBeRendered) {
			if (GetWorld()->GetTimeSeconds() - overlap.GetActor()->GetLastRenderTime() > maxTargetHoldTime)
				continue;
		}

		float distance = FVector::Dist(overlap.GetComponent()->GetComponentLocation(), GetOwner()->GetActorLocation());
		if (distance < shortestDistance) {
			shortestDistance = distance;
			returnTarget = Cast<USceneComponent>(overlap.GetComponent());
		}
	}

	return returnTarget;
}

void UAutoTargeterComponent::ToggleLockOn() {
	if (!target.IsValid())
		return;

	bLockOn = !bLockOn;
}

void UAutoTargeterComponent::SetLockOn(bool LockOn) {
	if (!target.IsValid()) {
		bLockOn = false;
		return;
	}

	bLockOn = LockOn;
}

void UAutoTargeterComponent::SetTargetToNearest() {
	target = FindNearestTarget(targetRadius, true);
}

FVector UAutoTargeterComponent::GetPlanarLookAtTargetVector() const {
	if (target.IsValid()) {
		FVector targetVector = target.Get()->GetComponentLocation() - GetOwner()->GetActorLocation();
		return FVector(targetVector.X, targetVector.Y, 0.0f).GetSafeNormal();
	}
	else
		return FVector::ZeroVector;
}