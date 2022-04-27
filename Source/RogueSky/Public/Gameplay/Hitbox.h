// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "Hitbox.generated.h"

class UHurtbox;
class UCombatComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHit, UHitbox*, HitboxHit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDestroyHitboxDelegate, UHitbox*, HitboxDestroyed);

UENUM()
enum class EKnockbackType : uint8 {
	Raw			UMETA(DisplayName = "Raw"),
	Relative	UMETA(DisplayName = "Relative"),
	Forward		UMETA(DisplayName = "Forward"),
};

USTRUCT()
struct FHitboxInfo {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FVector knockback;

	UPROPERTY(EditAnywhere)
		EKnockbackType knockbackType;

	FHitboxInfo() {
		knockback = FVector::ZeroVector;
		knockbackType = EKnockbackType::Raw;
	}

	FHitboxInfo(FVector Knockback, EKnockbackType knockbackType) {
		knockback = Knockback;
		knockbackType = knockbackType;
	}
};

USTRUCT()
struct FHitboxTransform {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FName socket;

	UPROPERTY(EditAnywhere)
		FVector location;

	UPROPERTY(EditAnywhere)
		FRotator rotation;

	UPROPERTY(EditAnywhere)
		float radius;

	UPROPERTY(EditAnywhere)
		float length;

	FHitboxTransform() {
		socket = "None";
		location = FVector::ZeroVector;
		rotation = FRotator::ZeroRotator;
		radius = 32.0f;
		length = 32.0f;
	}

	FHitboxTransform(FName Socket, FVector Location, FRotator Rotation, float Radius, float Length) {
		socket = Socket;
		location = Location;
		rotation = Rotation;
		radius = Radius;
		length = Length;
	}
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROGUESKY_API UHitbox : public UCapsuleComponent {
	GENERATED_BODY()

public:
	UHitbox();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UCombatComponent* combatComponent;

	UPROPERTY(EditAnywhere)
		FHitboxInfo hitboxInfo;

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);
	void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	
public:
	UPROPERTY(BlueprintAssignable)
		FOnHit OnHit;
	UPROPERTY(BlueprintAssignable)
		FDestroyHitboxDelegate OnHitboxDestroyed;

	void SetHitboxInfo(FHitboxInfo HitboxInfo) { hitboxInfo = HitboxInfo; }
	void SetKnockback(FVector Knockback) { hitboxInfo.knockback = Knockback; }
	void SetKnockbackType(EKnockbackType Type) { hitboxInfo.knockbackType = Type; }

	FVector GetKnockback() const { return hitboxInfo.knockback; }
	EKnockbackType GetKnockbackType() const { return hitboxInfo.knockbackType; }
	void SetCombatComponent(UCombatComponent* CombatComponent) { combatComponent = CombatComponent; }
	const UCombatComponent* GetCombatComponent() const { return combatComponent; }
};
