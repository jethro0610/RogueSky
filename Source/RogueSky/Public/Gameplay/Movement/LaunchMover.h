// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mover.h"
#include "Gameplay/Interaction/TargeterComponent.h"
#include "LaunchMover.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartLaunch);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndLaunch);

UCLASS(meta = (BlueprintSpawnableComponent))
class ROGUESKY_API ULaunchMover : public UMover
{
	GENERATED_BODY()
	
public:
	ULaunchMover();

protected:
	void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
		float verticalLaunchVelocity = 50.0f;
	UPROPERTY(EditAnywhere)
		float gravityMultiplier = 1.0f;
	UPROPERTY(EditAnywhere)
		float headTraceDistance = 60.0f;
	UPROPERTY(EditAnywhere)
		float trackingSpeed = 0.0f;
	UPROPERTY(EditAnywhere)
		float rollSpeed = 0.0f;
	UPROPERTY(EditAnywhere)
		float distanceToStopTracking = 0.0f;

	UTargeterComponent* targeterComponent;

public:
	void OnActivate_Implementation(FVector DesiredMovement) override;
	void OnDeactivate_Implementation() override;
	void DoMovement_Implementation(float DeltaTime, FVector DesiredMovement) override;

	bool OnEnterGround() override { return false; }
	bool OnExitGround() override { return false; }

	UPROPERTY(BlueprintAssignable)
		FOnStartLaunch OnStartLaunch;
	UPROPERTY(BlueprintAssignable)
		FOnEndLaunch OnEndLaunch;

private:
	FVector GetPlanarVelocityToTarget();
	FCollisionQueryParams collisionParams;

	float roll;

	bool hasTarget = false;
	UFUNCTION()
		void OnLoseTarget() { hasTarget = false; }
};
