// Fill out your copyright notice in the Description page of Project Settings.
#include "Gameplay/Movers/KnockbackMover.h"

UKnockbackMover::UKnockbackMover() {
    priority = 2;
}

void UKnockbackMover::BeginPlay() {
    Super::BeginPlay();
    combatComponent = GetOwner()->FindComponentByClass<UCombatComponent>();
    combatComponent->OnHitByAttack.AddDynamic(this, &UKnockbackMover::OnHitByAttack);
}

void UKnockbackMover::DoMovement_Implementation(float DeltaTime, FVector DesiredMovement) {
    if (movementComponent->GetVelocity().Size() > (currentKnockback.Size() * 0.035f) + 0.001f && !bFinishedKnockback) {
        FVector newVelocity = movementComponent->GetPlanarVelocity() * FMath::Pow(0.85f, DeltaTime * 120.0f);
        movementComponent->SetPlanarVelocity(newVelocity);
        if (movementComponent->GetGravity() < 0.0f) {
            float newGravity = movementComponent->GetGravity() * FMath::Pow(0.85f, DeltaTime * 120.0f);
            movementComponent->SetGravity(newGravity);
        }
    }
    else {
        bFinishedKnockback = true;
        if (movementComponent->IsOnGround() && movementComponent->GetGravity() >= 0.0f)
            movementComponent->UseQueuedMover();
    }
}

void UKnockbackMover::OnActivate_Implementation(FVector DesiredMovement) {
    movementComponent->bFriction = false;
    movementComponent->gravitySpeed /= 2.0f;
}

void UKnockbackMover::OnDeactivate_Implementation() {
    bFinishedKnockback = true;
    movementComponent->bFriction = true;
    movementComponent->gravitySpeed *= 2.0f;
}

void UKnockbackMover::OnHitByAttack(UHitbox* Hitbox, UHurtbox* Hurtbox) {
    if (Hitbox->GetKnockbackType() == EKnockbackType::Raw) {
        currentKnockback = Hitbox->GetKnockback();
    }
    else if (Hitbox->GetKnockbackType() == EKnockbackType::Relative) {
        FVector forwardVector = (GetOwner()->GetActorLocation() - Hitbox->GetOwner()->GetActorLocation());
        forwardVector -= FVector(0.0f, 0.0f, forwardVector.Z);
        forwardVector = forwardVector.GetSafeNormal();
        FVector knockbackVector = forwardVector * Hitbox->GetKnockback().Y + FVector(0.0f, 0.0f, Hitbox->GetKnockback().Z);
        ApplyKnockback(knockbackVector);
    }
    else if (Hitbox->GetKnockbackType() == EKnockbackType::Forward) {
        FVector knockbackVector =
            Hitbox->GetOwner()->GetActorRightVector() * Hitbox->GetKnockback().X +
            Hitbox->GetOwner()->GetActorForwardVector() * Hitbox->GetKnockback().Y +
            Hitbox->GetOwner()->GetActorUpVector() * Hitbox->GetKnockback().Z;
        ApplyKnockback(knockbackVector);
    }
}

void UKnockbackMover::ApplyKnockback(FVector KnockbackVector) {
    currentKnockback = KnockbackVector;
    bFinishedKnockback = false;
    movementComponent->SetVelocity(currentKnockback);
    movementComponent->SetActiveMover(this);
}