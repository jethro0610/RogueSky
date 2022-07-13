// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mover.h"
#include "SkateMover.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class ROGUESKY_API USkateMover : public UMover
{
	GENERATED_BODY()
	
public:
	USkateMover();
	
private:
	UPROPERTY(EditAnywhere)
		float maxSkateSpeed = 35.0f;
	UPROPERTY(EditAnywhere)
		float rotationSpeed = 1.5f;
	UPROPERTY(EditAnywhere)
		float downSlopeAcceleration = 0.035f;

public:
	void OnActivate_Implementation(FVector DesiredMovement) override;
	void OnDeactivate_Implementation() override;

	void DoGroundMovement_Implementation(float DeltaTime, FVector DesiredMovement) override;
	void DoAerialMovement_Implementation(float DeltaTime, FVector DesiredMovement) override;
	float GetSlopeBoost(FHitResult GroundTrace) const;

private:
	float skateSpeed;
	FVector direction;
};
