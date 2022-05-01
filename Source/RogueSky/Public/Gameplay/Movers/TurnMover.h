// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Movers/Mover.h"
#include "TurnMover.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class ROGUESKY_API UTurnMover : public UMover {
	GENERATED_BODY()
	
public:
	UTurnMover();

private:
	UPROPERTY(EditAnywhere)
		float turnSpeed = 10.0f;

	float leftTurnAmount = 0.0f;
	float rightTurnAmount = 0.0f;
public:
	UFUNCTION(BlueprintCallable)
		void SetLeftTurnAmount(float Amount) { leftTurnAmount = Amount; }
	UFUNCTION(BlueprintCallable)
		void SetRightTurnAmount(float Amount) { rightTurnAmount = Amount; }
	UFUNCTION(BlueprintCallable)
		void SetTurnAmounts(float LeftAmount, float RightAmount) {
			leftTurnAmount = LeftAmount; 
			rightTurnAmount = RightAmount;
		}
	void DoGroundMovement_Implementation(float DeltaTime, FVector DesiredMovement);
	void OnActivate_Implementation(FVector DesiredMovement);
	void OnDeactivate_Implementation();
};
