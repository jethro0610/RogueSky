// Fill out your copyright notice in the Description page of Project Settings.
#include "Gameplay/Notifies/AnimNotify_SpawnProjectile.h"

void UAnimNotify_SpawnProjectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
    UCombatComponent* combatComponent = MeshComp->GetOwner()->FindComponentByClass<UCombatComponent>();
    if (combatComponent != nullptr)
        combatComponent->SpawnProjectile(projectile, spawnOffset);

    Received_Notify(MeshComp, Animation, EventReference);
}
