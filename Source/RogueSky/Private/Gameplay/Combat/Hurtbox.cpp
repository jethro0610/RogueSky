// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Hurtbox.h"
#include "Gameplay/Hitbox.h"
#include "Gameplay/CombatComponent.h"

UHurtbox::UHurtbox() {
    SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
    SetCollisionResponseToAllChannels(ECR_Ignore);
    SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECR_Overlap);
}

void UHurtbox::BeginPlay() {
   Super::BeginPlay();
   combatComponent = GetOwner()->FindComponentByClass<UCombatComponent>();
}