// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Gameplay/Combat/Hitbox.h"
#include "AnimNotifyState_SpawnHitbox.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "Spawn Hitbox"))
class ROGUESKY_API UAnimNotifyState_SpawnHitbox : public UAnimNotifyState, public FTickableGameObject
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override { return true; }
	virtual bool IsTickableInEditor() const override { return true; }
	virtual bool IsTickableWhenPaused() const override { return true; }
	virtual TStatId GetStatId() const override { return UObject::GetStatID(); }

private:
	UHitbox* hitbox;

	UPROPERTY(EditAnywhere)
		FHitboxTransform hitboxTransform;

	UPROPERTY(EditAnywhere)
		FHitboxInfo hitboxInfo;

	float notifyStartTime = 0.0f;
	float notifyEndTime = 0.0f;
	TWeakObjectPtr<USkeletalMeshComponent> meshComponent;
	TWeakObjectPtr<UAnimMontage> animMontage;
	bool bShouldDraw = false;

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* AnimationSequence, float TotalDuration, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyTick(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* AnimationSequence, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* AnimationSequence, const FAnimNotifyEventReference& EventReference);
};
