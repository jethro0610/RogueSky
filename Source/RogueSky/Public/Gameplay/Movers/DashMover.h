// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Movers/Mover.h"
#include "DashMover.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndDash);

UCLASS(meta = (BlueprintSpawnableComponent))
class ROGUESKY_API UDashMover : public UMover
{
	GENERATED_BODY()
	
public:
	UDashMover();

private:
	UPROPERTY(EditAnywhere)
		float minDashSpeed = 0.0f;

	UPROPERTY(EditAnywhere)
		float easeInStrength = 2.0f;

	UPROPERTY(EditAnywhere)
		float initialRotationInfluence = 0.0f;

	UPROPERTY(EditAnywhere)
		float rotationSpeed = 2.0f;

	UPROPERTY(EditAnywhere)
		float dashSpeedMultiplier = 1.0f;

	UPROPERTY(EditAnywhere)
		float dashLength = 0.5f;

	UPROPERTY(EditAnywhere)
		float exitSpeed = 7.0f;

	float dashTimer;
	float dashSpeed;

public:
	UPROPERTY(BlueprintAssignable)
		FOnEndDash OnEndDash;

	void OnActivate_Implementation(FVector DesiredMovement);
	void OnDeactivate_Implementation();
	void DoMovement_Implementation(float DeltaTime, FVector DesiredMovement);
};
