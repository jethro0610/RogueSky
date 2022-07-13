// Fill out your copyright notice in the Description page of Project Settings.
#include "Gameplay/Combat/CombatComponent.h"

UCombatComponent::UCombatComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (hitlag > 0.0f)
        hitlag -= DeltaTime;
}

UHitbox* UCombatComponent::SpawnHitbox(USceneComponent* Parent, FHitboxTransform HitboxTransform, FHitboxInfo HitboxInfo) {
	UHitbox* spawnedHitbox = NewObject<UHitbox>(this, UHitbox::StaticClass());
	spawnedHitbox->AttachToComponent(Parent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, HitboxTransform.socket);
	spawnedHitbox->SetAbsolute(false, false, true);
	spawnedHitbox->AddLocalOffset(HitboxTransform.location * Parent->GetSocketTransform(HitboxTransform.socket).GetScale3D());
	spawnedHitbox->AddLocalRotation(HitboxTransform.rotation);
	spawnedHitbox->SetCapsuleSize(HitboxTransform.radius, HitboxTransform.length, true);
	spawnedHitbox->SetHitboxInfo(HitboxInfo);
	spawnedHitbox->SetCombatComponent(this);
	spawnedHitbox->RegisterComponent();

	return spawnedHitbox;
}

AProjectile* UCombatComponent::SpawnProjectile( TSubclassOf<AProjectile> Class, FVector Offset) {
	FVector spawnOffset =
		GetOwner()->GetActorRightVector() * Offset.X +
		GetOwner()->GetActorForwardVector() * Offset.Y +
		GetOwner()->GetActorUpVector() * Offset.Z;

	FVector spawnLocation = GetOwner()->GetActorLocation() + spawnOffset;
	AProjectile* spawnedProjectile = (AProjectile*) GetWorld()->SpawnActor(Class.Get(), &spawnLocation);
	spawnedProjectile->SetCombatComponent(this);

	return spawnedProjectile;
}

bool UCombatComponent::OnHitboxContactHurtbox(UHitbox* Hitbox, UHurtbox* Hurtbox) {
	if (Hurtbox->GetCombatComponent() != nullptr) {
		if (Hurtbox->GetCombatComponent()->contactedHitboxes.Contains(Hitbox))
			return false;
	}

	//if(Hitbox->GetOwner() == GetOwner())
		//ApplyHitlag(0.00f);

	OnLandAttack.Broadcast(Hitbox, Hurtbox);
	return true;
}

bool UCombatComponent::OnHurtboxContactHitbox(UHitbox* Hitbox, UHurtbox* Hurtbox) {
	if (contactedHitboxes.Contains(Hitbox))
		return false;

	Hitbox->OnHitboxDestroyed.AddDynamic(this, &UCombatComponent::OnHitboxDestroyed);
	//ApplyHitlag(0.00f);
	OnHitByAttack.Broadcast(Hitbox, Hurtbox);
	contactedHitboxes.Add(Hitbox);
	return true;

}

void UCombatComponent::OnHitboxDestroyed(UHitbox* Hitbox) {
	contactedHitboxes.Remove(Hitbox);
}