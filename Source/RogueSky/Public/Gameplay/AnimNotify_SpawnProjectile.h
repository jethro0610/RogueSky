// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CombatComponent.h"
#include "AnimNotify_SpawnProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ROGUESKY_API UAnimNotify_SpawnProjectile : public UAnimNotify
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
		TSubclassOf<AProjectile> projectile;
	
	UPROPERTY(EditAnywhere)
		FVector spawnOffset;

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
