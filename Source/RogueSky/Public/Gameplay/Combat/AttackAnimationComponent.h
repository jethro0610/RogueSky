// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VelocityMovementComponent.h"
#include "CombatComponent.h"
#include "AttackAnimationComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartAttackDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndAttackingDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUESKY_API UAttackAnimationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttackAnimationComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	USkeletalMeshComponent* skeletalMesh;
	UVelocityMovementComponent* movementComponent;
	UCombatComponent* combatComponent;
	bool bInterruptable = false;

	FOnMontageEnded EndDelegate;
	void MontageEnd(UAnimMontage* Montage, bool bInterrupted);

public:
	UPROPERTY(BlueprintAssignable)
		FStartAttackDelegate OnStartAttack;
	UPROPERTY(BlueprintAssignable)
		FEndAttackingDelegate OnEndAttack;

	UFUNCTION(BlueprintCallable)
		bool IsAttacking() const;

	UFUNCTION(BlueprintCallable)
		void SetInterruptable(bool CanInterruptAttack) { bInterruptable = CanInterruptAttack; }

	UFUNCTION(BlueprintCallable)
		bool DoAttack(UAnimMontage* AttackMontage);
};
