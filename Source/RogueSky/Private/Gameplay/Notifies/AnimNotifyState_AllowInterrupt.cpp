// Fill out your copyright notice in the Description page of Project Settings.
#include "Gameplay/Notifies/AnimNotifyState_AllowInterrupt.h"
#include "Gameplay/Combat/AttackAnimationComponent.h"

void UAnimNotifyState_AllowInterrupt::NotifyBegin(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* AnimationSequence, float TotalDuration, const FAnimNotifyEventReference& EventReference) {
	UAttackAnimationComponent* attackAnimationComponent = MeshComponent->GetOwner()->FindComponentByClass<UAttackAnimationComponent>();
	if (attackAnimationComponent != nullptr)
		attackAnimationComponent->SetInterruptable(true);

	Received_NotifyBegin(MeshComponent, AnimationSequence, TotalDuration, EventReference);
}


void UAnimNotifyState_AllowInterrupt::NotifyEnd(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* AnimationSequence, const FAnimNotifyEventReference& EventReference) {
	UAttackAnimationComponent* attackAnimationComponent = MeshComponent->GetOwner()->FindComponentByClass<UAttackAnimationComponent>();
	if (attackAnimationComponent != nullptr)
		attackAnimationComponent->SetInterruptable(false);

	Received_NotifyEnd(MeshComponent, AnimationSequence, EventReference);
}