// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mover.h"
#include "Gameplay/Interaction/TargeterComponent.h"
#include "TrackTargetMover.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class ROGUESKY_API UTrackTargetMover : public UMover
{
	GENERATED_BODY()
	
public:
	UTrackTargetMover();

private:
	void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		bool bTrackZ = false;

	UPROPERTY(EditAnywhere)
		float trackingSpeed = 1.5f;

	UPROPERTY(EditAnywhere)
		float desiredSpeed = 0.0f;

	UPROPERTY(EditAnywhere)
		float lerpSpeed = 0.0f;

	UTargeterComponent* targeterComponent;
	bool hasTarget = false;
	UFUNCTION()
		void OnLoseTarget() { hasTarget = false; } 

public:
	void OnActivate_Implementation(FVector DesiredMovement);
	void OnDeactivate_Implementation();
	void DoMovement_Implementation(float DeltaTime, FVector DesiredMovement);

	UFUNCTION(BlueprintCallable)
		void StopTracking();
};
