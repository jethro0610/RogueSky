// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Movers/Mover.h"
#include "Gameplay/CombatComponent.h"
#include "KnockbackMover.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class ROGUESKY_API UKnockbackMover : public UMover
{
	GENERATED_BODY()
	
public:
	UKnockbackMover();

protected:
	virtual void BeginPlay() override;

public:
	void OnActivate_Implementation(FVector DesiredMovement) override;
	void OnDeactivate_Implementation() override;

	void DoMovement_Implementation(float DeltaTime, FVector DesiredMovement) override;
	bool OnExitGround() override { return false; }

private:
	UCombatComponent* combatComponent;
	bool bFinishedKnockback;
	FVector currentKnockback;

	UFUNCTION()
		void OnHitByAttack(UHitbox* Hitbox, UHurtbox* Hurtbox);

	void ApplyKnockback(FVector KnockbackVector);
};
