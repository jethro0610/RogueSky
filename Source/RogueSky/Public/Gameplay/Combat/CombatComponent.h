// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Hitbox.h"
#include "Hurtbox.h"
#include "CombatComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHitByAttackDelegate, UHitbox*, Hitbox, UHurtbox*, Hurtbox);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLandAttackDelegate, UHitbox*, Hitbox, UHurtbox*, Hurtbox);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUESKY_API UCombatComponent : public UActorComponent {
	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	UCombatComponent();

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	friend UHitbox;
	friend UHurtbox;

private:
	float hitlag = 0.0f;
	TSet<UHitbox*> contactedHitboxes;

	void ApplyHitlag(float Hitlag) { hitlag = Hitlag; }
	bool OnHitboxContactHurtbox(UHitbox* Hitbox, UHurtbox* Hurtbox);
	bool OnHurtboxContactHitbox(UHitbox* Hitbox, UHurtbox* Hurtbox);

	UFUNCTION()
		void OnHitboxDestroyed(UHitbox* Hitbox);
public:
	UPROPERTY(BlueprintAssignable)
		FHitByAttackDelegate OnHitByAttack;
	UPROPERTY(BlueprintAssignable)
		FLandAttackDelegate OnLandAttack;
	bool IsInHitlag() const { return hitlag > 0.0f; }
	UHitbox* SpawnHitbox(USceneComponent* Parent, FHitboxTransform HitboxTransform, FHitboxInfo HitboxInfo);
};
