// Fill out your copyright notice in the Description page of Project Settings.
#include "Gameplay/Combat/Hurtbox.h"
#include "Gameplay/Combat/Hitbox.h"
#include "Gameplay/Combat/CombatComponent.h"

UHurtbox::UHurtbox() {
    SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
    SetCollisionResponseToAllChannels(ECR_Ignore);
    SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECR_Overlap);
}

void UHurtbox::BeginPlay() {
   Super::BeginPlay();
   combatComponent = GetOwner()->FindComponentByClass<UCombatComponent>();
}