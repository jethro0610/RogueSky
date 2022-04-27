// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickupInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickupInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ROGUESKY_API IPickupInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
		void OnPickup(USceneComponent* ComponentToAttachTo);

	UFUNCTION(BlueprintNativeEvent)
		void OnUse(USceneComponent* Target, FVector ForwardVector);

	UFUNCTION(BlueprintNativeEvent)
		bool CanPickup();

	UFUNCTION(BlueprintNativeEvent)
		FName GetPickupName();
};
