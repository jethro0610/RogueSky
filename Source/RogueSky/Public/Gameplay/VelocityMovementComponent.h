// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "RogueSkyGameModeBase.h"
#include "VelocityMovementComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FEnterGroundDelegate, FHitResult, FVector);
DECLARE_MULTICAST_DELEGATE(FExitGroundDelegate);

class UMover;

UCLASS(meta = (BlueprintSpawnableComponent))
class ROGUESKY_API UVelocityMovementComponent : public UMovementComponent
{
	GENERATED_BODY()
	
public:
	UVelocityMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere)
		float groundFriction = 0.075;
	UPROPERTY(EditAnywhere)
		float airFriction = 0.05f;
	UPROPERTY(EditAnywhere)
		float maxGravity = 5000.0f;
	UPROPERTY(EditAnywhere)
		float gravitySpeed = 0.5f;
	UPROPERTY(EditAnywhere)
		bool bFriction = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bGravity = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bStickToGround = true;

private:
	UPROPERTY(EditAnywhere)
		bool bMovementEnabled = true;

	FVector velocity;
	FVector trueVelocity;

	FVector previousLocation;
	FVector currentLocation;

	FCollisionQueryParams groundTraceParams;
	float halfHeight;
	float groundDistance;
	float groundTraceDistance;
	bool bGroundedLastTick = false;
	FHitResult groundTrace;

	FVector desiredMovement;
	float desiredYawDelta;
	FVector lastPlanarVelocity;
	float planarDotProduct;
	UMover* activeMover;
	UMover* queuedMover;

public:
	UFUNCTION(BlueprintCallable)
		void SetMovementEnabled(bool bEnableMovement);

	UFUNCTION(BlueprintCallable)
		void SetDesiredMovement(FVector DesiredMovement);
	UFUNCTION(BlueprintCallable)
		void SetActiveMover(UMover* mover);
	UFUNCTION(BlueprintPure)
		UMover* GetActiveMover() const { return activeMover; }
	UFUNCTION(BlueprintCallable)
		void UseQueuedMover();
	UFUNCTION(BlueprintPure)
		float GetDesiredYawDelta() const { return desiredYawDelta; }
	UFUNCTION(BlueprintPure)
		float GetPlanarDotProduct() const { return planarDotProduct; }

	UFUNCTION(BlueprintCallable)
		void UpdateGroundTraceDistance() { groundTraceDistance = halfHeight + groundDistance; }

	UFUNCTION(BlueprintCallable)
		void SetHalfHeight(float HalfHeight) { 
			halfHeight = HalfHeight;  
			UpdateGroundTraceDistance();
		}

	UFUNCTION(BlueprintCallable)
		void SetGroundDistance(float NewGroundDistance) {
			groundDistance = NewGroundDistance;
			UpdateGroundTraceDistance();
		}
	
	UFUNCTION(BlueprintCallable)
		FHitResult CalculateGroundTrace();
	UFUNCTION(BlueprintPure)
		FHitResult GetGroundTrace() const { return groundTrace; }
	UFUNCTION(BlueprintPure)
		bool IsOnGround() const { return GetGroundTrace().IsValidBlockingHit(); }
	UFUNCTION(BlueprintPure)
		FRotator GetGroundAlignRotator() const;

	UFUNCTION(BlueprintPure)
		FVector GetVelocity() const { return velocity; }
	UFUNCTION(BlueprintPure)
		FVector GetPlanarVelocity() const { return FVector(velocity.X, velocity.Y, 0.0f); }
	UFUNCTION(BlueprintCallable)
		void SetVelocity(FVector NewVelocity) { velocity = NewVelocity; trueVelocity = NewVelocity; }
	UFUNCTION(BlueprintCallable)
		void SetPlanarVelocity(FVector NewVelocity) { velocity = FVector(NewVelocity.X, NewVelocity.Y, velocity.Z); }
	UFUNCTION(BlueprintCallable)
		void AddVelocity(FVector DeltaVelocity) { velocity += DeltaVelocity; }
	UFUNCTION(BlueprintCallable)
		void AddPlanarVelocity(FVector DeltaVelocity) { velocity += FVector(DeltaVelocity.X, DeltaVelocity.Y, 0.0f); }

	UFUNCTION(BlueprintPure)
		float GetGravity() const { return -velocity.Z; };
	UFUNCTION(BlueprintCallable)
		void SetGravity(float NewGravity) { velocity.Z = -NewGravity; }
	UFUNCTION(BlueprintCallable)
		void AddGravity(float DeltaGravity) { velocity.Z -= DeltaGravity; };

	UFUNCTION(BlueprintCallable)
		void SetPositionZ(float NewZ);
	UFUNCTION(BlueprintCallable)
		virtual void Move(FVector DeltaLocation);

	FEnterGroundDelegate EnterGround;
	virtual void OnEnterGround(FHitResult GroundTrace, FVector EntryVector);

	FExitGroundDelegate ExitGround;
	virtual void OnExitGround();

	UFUNCTION(BlueprintCallable)
		void SetFootLocation(FVector Location);
};
