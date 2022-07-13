// Fill out your copyright notice in the Description page of Project Settings.
#include "Gameplay/Combat/Projectile.h"
#include "Gameplay/Combat/CombatComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collider = CreateDefaultSubobject<USphereComponent>("Collider");
	collider->SetCollisionProfileName("BlockAllDynamic");

	movementComponent = CreateDefaultSubobject<UVelocityMovementComponent>("Movement");
	movementComponent->bFriction = false;
	movementComponent->bGravity = false;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	TArray<UHitbox*> hitboxComponents;
	GetComponents<UHitbox>(hitboxComponents);
	for (UHitbox* hitbox : hitboxComponents) {
		hitbox->OnHit.AddDynamic(this, &AProjectile::OnHit);
		hitboxes.Add(Cast<UHitbox>(hitbox));
	}
	movementComponent->SetHalfHeight(collider->GetScaledSphereRadius());
	// TOADD: movementComponent->StickToGround(false);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (hitlag > 0.0f) {
		hitlag -= DeltaTime;
	}
	else {
		if (bShouldDestroy)
			Destroy();
	}
}

void AProjectile::SetCombatComponent(UCombatComponent* CombatComponent) {
	if (combatComponent == nullptr) {
		combatComponent = CombatComponent;
		InitProjectileMotion();
	}
	else
		combatComponent = CombatComponent;

	for (UHitbox* hitbox : hitboxes)
		hitbox->SetCombatComponent(CombatComponent);
}
