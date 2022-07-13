// Fill out your copyright notice in the Description page of Project Settings.
#include "Gameplay/Combat/Hitbox.h"
#include "Gameplay/Combat/Hurtbox.h"
#include "Gameplay/Combat/CombatComponent.h"

UHitbox::UHitbox() {
    SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
    SetCollisionResponseToAllChannels(ECR_Ignore);
    SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECR_Overlap);
}

void UHitbox::BeginPlay() {
   Super::BeginPlay();
   OnComponentBeginOverlap.AddDynamic(this, &UHitbox::OnBeginOverlap);
}

void UHitbox::OnBeginOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult) {
   if (OtherActor != GetOwner()) {
      UHurtbox* overlappedHurtbox = Cast<UHurtbox>(OtherComponent);
      if (overlappedHurtbox != nullptr) {
          if (overlappedHurtbox->combatComponent != nullptr && overlappedHurtbox->combatComponent != combatComponent)
              overlappedHurtbox->combatComponent->OnHurtboxContactHitbox(this, overlappedHurtbox);

          // Broadcast a hit if the hitbox registers on the combat component, or broadcast a hit if there is no combat component
          if (combatComponent != nullptr && combatComponent != overlappedHurtbox->combatComponent) {
              if (combatComponent->OnHitboxContactHurtbox(this, overlappedHurtbox))
                  OnHit.Broadcast(this);
          }
          else {
              OnHit.Broadcast(this);
          }
      }
   }
}

void UHitbox::OnComponentDestroyed(bool bDestroyingHierarchy) {
    OnHitboxDestroyed.Broadcast(this);
    Super::OnComponentDestroyed(bDestroyingHierarchy);
}