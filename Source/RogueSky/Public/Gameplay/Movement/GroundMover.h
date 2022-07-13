// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mover.h"
#include "GroundMover.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class ROGUESKY_API UGroundMover : public UMover
{
	GENERATED_BODY()

protected:
	void BeginPlay() override;

public:
	UGroundMover();

	UPROPERTY(EditAnywhere)
		float maxSpeed = 6.0f;

	UPROPERTY(EditAnywhere)
		bool bUseForwardWalk = false;

	UPROPERTY(EditAnywhere)
		float rotationSpeed = 12.0f;

	UPROPERTY(EditAnywhere)
		bool bTransferMomentum = false;

	UPROPERTY(EditAnywhere)
		float momentumDecay = 0.025f;

	UPROPERTY(EditAnywhere)
		float momentumCancelStrength = 0.5f;

	UPROPERTY(EditAnywhere)
		float peakSpeedForFriction = 35.0f;

	UPROPERTY(EditAnywhere)
		float minimumMomentumFriction = 0.01f;

	float defaultMaxSpeed;
	float defaultFriction;
public:
	void OnActivate_Implementation(FVector DesiredMovement);
	void DoGroundMovement_Implementation(float DeltaTime, FVector DesiredMovement) override;
	void DoAerialMovement_Implementation(float DeltaTime, FVector DesiredMovement) override;

	UFUNCTION(BlueprintPure)
		float GetSpeedScalar() const { return movementComponent->GetPlanarVelocity().Size() / defaultMaxSpeed; }
};
