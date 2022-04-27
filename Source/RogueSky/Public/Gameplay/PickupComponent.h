// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "PickupInterface.h"
#include "TargeterComponent.h"
#include "PickupComponent.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FHoldSlotInfo {
	GENERATED_BODY()

public:
	FName itemName = "";
	int amount = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartPickup);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartUse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopUse);

UCLASS(meta = (BlueprintSpawnableComponent))
class ROGUESKY_API UPickupComponent : public USphereComponent
{
	GENERATED_BODY()
	
public:
	UPickupComponent();

private:
	void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable)
		void StartPickingUp(int HoldSlot);

	UFUNCTION(BlueprintCallable)
		void StopPickingUp();

	UFUNCTION(BlueprintCallable)
		void ActivateHoldSlot(int HoldSlot);

	UFUNCTION(BlueprintCallable)
		void PickupActor(AActor* ActorToPickup, int HoldSlot);

	UFUNCTION(BlueprintPure)
		FHoldSlotInfo GetHoldSlotInfo(int HoldSlot) const;

	UFUNCTION(BlueprintPure)
		int GetHoldSlotAmount(int HoldSlot) const;

	UFUNCTION(BlueprintPure)
		FName GetHoldSlotItemName(int HoldSlot) const;

	UFUNCTION(BlueprintPure)
		bool IsPickingUp() const { return pickupHoldSlot != -1; }

	UFUNCTION(BlueprintPure)
		bool IsUsing() const { return holdSlotToUse != -1; }

	UFUNCTION(BlueprintCallable)
		void StartUsing(int HoldSlot = -1);

	UFUNCTION(BlueprintCallable)
		void StopUsing();

	UFUNCTION(BlueprintCallable)
		void UseHoldSlot(int HoldSlot = -1);

	UFUNCTION(BlueprintCallable)
		void SetUseInterruptable(bool Interruptable);

	UFUNCTION(BlueprintPure)
		int GetUseComboCount() const { return useCombo; }

	UPROPERTY(BlueprintAssignable)
		FOnStartPickup OnStartPickup;

	UPROPERTY(BlueprintAssignable)
		FOnStartUse OnStartUse;

	UPROPERTY(BlueprintAssignable)
		FOnStopUse OnStopUse;

private:
	UPROPERTY(EditAnywhere)
		bool bUseImmediatly = true;

	UPROPERTY(EditAnywhere)
		int maxUseCombo = 4;

	int useCombo = 0;
	int pickupHoldSlot = -1;
	int holdSlotToUse = -1;
	bool bUseInterruptable = false;

	TArray<TWeakObjectPtr<AActor>> heldActors[4];
	TSet<TWeakObjectPtr<AActor>> overlappedActors;
	UTargeterComponent* targeter;

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

	UFUNCTION()
		void OnActorDestroyed(AActor* DestroyedActor);
};
