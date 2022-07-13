// Fill out your copyright notice in the Description page of Project Settings.
#include "Gameplay/Notifies/AnimNotifyState_SpawnHitbox.h"
#include "Gameplay/Combat/CombatComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

void UAnimNotifyState_SpawnHitbox::NotifyBegin(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* AnimationSequence, float TotalDuration, const FAnimNotifyEventReference& EventReference) {
	UCombatComponent* combatComponent = MeshComponent->GetOwner()->FindComponentByClass<UCombatComponent>();
	if (combatComponent != nullptr)
		hitbox = combatComponent->SpawnHitbox(MeshComponent, hitboxTransform, hitboxInfo);

	Received_NotifyBegin(MeshComponent, AnimationSequence, TotalDuration, EventReference);
}

void UAnimNotifyState_SpawnHitbox::NotifyTick(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* AnimationSequence, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) {
	if (GIsEditor) {
		for (int i = 0; i < MeshComponent->GetAnimInstance()->ActiveAnimNotifyState.Num(); i++) {
			if (MeshComponent->GetAnimInstance()->ActiveAnimNotifyState[i].NotifyStateClass == this) {
				notifyStartTime = MeshComponent->GetAnimInstance()->ActiveAnimNotifyState[i].GetTriggerTime();
				notifyEndTime = MeshComponent->GetAnimInstance()->ActiveAnimNotifyState[i].GetEndTriggerTime();
			}
		}
		meshComponent = MeshComponent;
		animMontage = Cast<UAnimMontage, UAnimSequenceBase>(AnimationSequence);
	}
		
	Received_NotifyTick(MeshComponent, AnimationSequence, FrameDeltaTime, EventReference);
}

void UAnimNotifyState_SpawnHitbox::Tick(float DeltaTime) {
	if (!GIsEditor)
		return;
	if (meshComponent == nullptr)
		return;
	if (meshComponent->GetWorld()->HasBegunPlay())
		return;
	if (animMontage == nullptr)
		return;

	float currentTime = meshComponent->GetAnimInstance()->Montage_GetPosition(animMontage.Get());
	if (currentTime >= notifyStartTime && currentTime <= notifyEndTime) {
		FTransform socketTransform = meshComponent->GetSocketTransform(hitboxTransform.socket);
		FVector drawLocation = socketTransform.TransformPosition(hitboxTransform.location / socketTransform.GetScale3D());
		FQuat drawRotation  = socketTransform.TransformRotation(hitboxTransform.rotation.Quaternion());
		DrawDebugCapsule(meshComponent->GetWorld(), drawLocation, hitboxTransform.length, hitboxTransform.radius, drawRotation, FColor::Red);
	}
}

void UAnimNotifyState_SpawnHitbox::NotifyEnd(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* AnimationSequence, const FAnimNotifyEventReference& EventReference) {
	if (hitbox != nullptr) {
		hitbox->DestroyComponent();
		hitbox = nullptr;
	}

	Received_NotifyEnd(MeshComponent, AnimationSequence, EventReference);
}