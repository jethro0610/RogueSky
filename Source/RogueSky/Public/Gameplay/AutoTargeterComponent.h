// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/TargeterComponent.h"
#include "AutoTargeterComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROGUESKY_API UAutoTargeterComponent : public UTargeterComponent
{
	GENERATED_BODY()

public:
	UAutoTargeterComponent();

private:
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetTargetLost() override;

	UPROPERTY(EditAnywhere)
		float targetRadius = 5000.0f;

	UPROPERTY(EditAnywhere)
		float maxTargetHoldTime = 30.0f;

	bool bLockOn = false;

public:
	FVector GetVectorToTarget() const override;

	UFUNCTION(BlueprintPure)
		bool IsLockedOn() const { return bLockOn; }

	UFUNCTION(BlueprintCallable)
		void ToggleLockOn();

	UFUNCTION(BlueprintCallable)
		void SetLockOn(bool LockOn);

	UFUNCTION(BlueprintPure)
		USceneComponent* FindNearestTarget(float Radius, bool bMustBeRendered = false) const;

	UFUNCTION(BlueprintPure)
		FVector GetPlanarLookAtTargetVector() const;

	UFUNCTION(BlueprintCallable)
		void SetTargetToNearest();
};
