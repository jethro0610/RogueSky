// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VelocityMovementComponent.h"
#include "Mover.generated.h"

UCLASS( Abstract, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUESKY_API UMover : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadWrite)
		UVelocityMovementComponent* movementComponent;
	bool bQueueable = false;
	int priority = -1;

public:
	UFUNCTION(BlueprintNativeEvent)
		void OnActivate(FVector DesiredMovement);
		virtual void OnActivate_Implementation(FVector DesiredMovement) { }

	UFUNCTION(BlueprintNativeEvent)
		void OnDeactivate();
		virtual void OnDeactivate_Implementation() { }

	UFUNCTION(BlueprintNativeEvent)
		void DoMovement(float DeltaTime, FVector DesiredMovement);
		virtual void DoMovement_Implementation(float DeltaTime, FVector DesiredMovement) {};

	UFUNCTION(BlueprintNativeEvent)
		void DoGroundMovement(float DeltaTime, FVector desiredMovement);
		virtual void DoGroundMovement_Implementation(float DeltaTime, FVector DesiredMovement) {};

	UFUNCTION(BlueprintNativeEvent)
		void DoAerialMovement(float DeltaTime, FVector desiredMovement);
		virtual void DoAerialMovement_Implementation(float DeltaTime, FVector DesiredMovement) {};

	UFUNCTION(BlueprintPure)
		int GetPriority() const { return priority; }

	UFUNCTION(BlueprintPure)
		bool IsQueueable() const { return bQueueable; }

	UFUNCTION(BlueprintPure)
		bool IsActiveMover() const { return movementComponent->GetActiveMover() == this; }

	virtual bool OnEnterGround() { return true; }
	virtual bool OnExitGround() { return true; }
};
