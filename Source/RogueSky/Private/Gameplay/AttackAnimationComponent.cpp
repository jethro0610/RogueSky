// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/AttackAnimationComponent.h"

// Sets default values for this component's properties
UAttackAnimationComponent::UAttackAnimationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UAttackAnimationComponent::BeginPlay()
{
	Super::BeginPlay();

	skeletalMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	movementComponent = GetOwner()->FindComponentByClass<UVelocityMovementComponent>();
	combatComponent = GetOwner()->FindComponentByClass<UCombatComponent>();
	EndDelegate.BindUObject(this, &UAttackAnimationComponent::MontageEnd);
	check(skeletalMesh != nullptr);
}


// Called every frame
void UAttackAnimationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Move with animation root motion
	if (movementComponent != nullptr) {
		FTransform rootTransform = skeletalMesh->ConsumeRootMotion().GetRootMotionTransform();
		movementComponent->Move(skeletalMesh->GetComponentRotation().RotateVector(rootTransform.GetLocation()));
	}

	if (combatComponent == nullptr)
		return;

	if (combatComponent->IsInHitlag())
		skeletalMesh->bPauseAnims = true;
	else
		skeletalMesh->bPauseAnims = false;
}

void UAttackAnimationComponent::MontageEnd(UAnimMontage* Montage, bool bInterrupted) {
	if (!bInterrupted) {
		bInterruptable = false;
		OnEndAttack.Broadcast();
	}
}

bool UAttackAnimationComponent::IsAttacking() const {
	if (skeletalMesh->GetAnimInstance()->GetCurrentActiveMontage() == nullptr)
		return false;
	else
		return true;
}

bool UAttackAnimationComponent::DoAttack(UAnimMontage* AttackMontage) {
	if (IsAttacking() && !bInterruptable)
		return false;

	// TODO: Return false here when the movement component's priority level is higher than the attack mover

	skeletalMesh->GetAnimInstance()->Montage_Play(AttackMontage);
	skeletalMesh->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate);
	OnStartAttack.Broadcast();
	return true;
}

