// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Gameplay/Movement/VelocityMovementComponent.h"
#include "Hitbox.h"
#include "Projectile.generated.h"

class UCombatComponent;

UCLASS()
class ROGUESKY_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere)
		USphereComponent* collider;

	float hitlag = 0.0f;
	bool bShouldDestroy = false;
	UCombatComponent* combatComponent;

	UPROPERTY(VisibleAnywhere)
		UVelocityMovementComponent* movementComponent;

	TArray<UHitbox*> hitboxes;
	AActor* target;

	UFUNCTION()
	virtual void OnHit(UHitbox* Hitbox) { 
		hitlag = 0.04f; 
		bShouldDestroy = true;
	}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void InitProjectileMotion();

	virtual void InitProjectileMotion_Implementation() {};

public:
	void SetCombatComponent(UCombatComponent* CombatComponent);
	void SetTarget(AActor* Target) { target = Target; }

	UFUNCTION(BlueprintPure)
		UCombatComponent* GetCombatComponent() const { return combatComponent; }
	UFUNCTION(BlueprintPure)
		UVelocityMovementComponent* GetMovementComponent() const { return movementComponent; }
};
