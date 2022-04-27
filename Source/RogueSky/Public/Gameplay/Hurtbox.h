// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "Hurtbox.generated.h"

class UHitbox;
class UCombatComponent;

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROGUESKY_API UHurtbox : public UCapsuleComponent {
	GENERATED_BODY()

private:
	friend UHitbox;

public:
	UHurtbox();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UCombatComponent* combatComponent;

public:
	const UCombatComponent* GetCombatComponent() const { return combatComponent; }
};
