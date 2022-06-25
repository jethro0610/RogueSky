// Fill out your copyright notice in the Description page of Project Settings.
#include "Gameplay/VelocityMovementComponent.h"
#include "Gameplay/Movers/Mover.h"
#include "Kismet/KismetMathLibrary.h"

UVelocityMovementComponent::UVelocityMovementComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UVelocityMovementComponent::BeginPlay() {
	Super::BeginPlay();
	EnterGround.AddUObject(this, &UVelocityMovementComponent::OnEnterGround);
	ExitGround.AddUObject(this, &UVelocityMovementComponent::OnExitGround);

	groundTraceParams.AddIgnoredActor(GetOwner());

	previousLocation = GetOwner()->GetActorLocation();
	currentLocation = previousLocation;
}

void UVelocityMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bMovementEnabled)
		return;

	if (desiredMovement.Size() > 0.1f)
		desiredYawDelta = FMath::FindDeltaAngleDegrees(GetOwner()->GetActorRotation().Yaw, desiredMovement.ToOrientationRotator().Yaw);
	else
		desiredYawDelta = 0.0f;

	CalculateGroundTrace();
	if (groundTrace.IsValidBlockingHit() && GetGravity() >= 0.0f) {
		if (activeMover != nullptr)
			activeMover->DoGroundMovement(DeltaTime, desiredMovement);

		if (!bGroundedLastTick) {
			bGroundedLastTick = true;
			EnterGround.Broadcast(groundTrace, velocity);
		}
		SetGravity(0.0f);
		SetGroundDistance(200.0f);

		if (bFriction) {
			FVector frictionVelocity = GetPlanarVelocity() * FMath::Exp(-groundFriction * DeltaTime * 120.0f);
			SetPlanarVelocity(frictionVelocity);
		}

		if (bStickToGround)
			SetPositionZ(groundTrace.Location.Z + halfHeight);
	}
	else {
		if (activeMover != nullptr)
			activeMover->DoAerialMovement(DeltaTime, desiredMovement);

		if (bGroundedLastTick) {
			bGroundedLastTick = false;
			ExitGround.Broadcast();
		}
		SetGroundDistance(FMath::Max(0.0f, GetGravity() + 1.0f));

		if (bGravity)
			AddGravity(gravitySpeed * DeltaTime * 120.0f);

		if (bFriction) {
			FVector frictionVelocity = GetPlanarVelocity() * FMath::Exp(-airFriction * DeltaTime * 120.0f);
			SetPlanarVelocity(frictionVelocity);
		}

		if (GetGravity() > maxGravity)
			SetGravity(maxGravity);
	}
	if (activeMover != nullptr)
		activeMover->DoMovement(DeltaTime, desiredMovement);

	Move(velocity * DeltaTime * 120.0f);

	previousLocation = currentLocation;
	currentLocation = GetOwner()->GetActorLocation();
	trueVelocity = (currentLocation - previousLocation) / (DeltaTime * 120.0f);
	planarDotProduct = FVector::DotProduct(lastPlanarVelocity.GetSafeNormal(), GetPlanarVelocity().GetSafeNormal());
	lastPlanarVelocity = GetPlanarVelocity();
}

void UVelocityMovementComponent::SetMovementEnabled(bool bEnableMovement) {
	if (bEnableMovement && !bMovementEnabled) {
		bMovementEnabled = true;
		SetVelocity(FVector::ZeroVector);
		previousLocation = currentLocation;
		currentLocation = GetOwner()->GetActorLocation();
		trueVelocity = FVector::ZeroVector;
		lastPlanarVelocity = GetPlanarVelocity();
	}
	else if (!bEnableMovement && bMovementEnabled) {
		bMovementEnabled = false;
		SetVelocity(FVector::ZeroVector);
	}
}

FHitResult UVelocityMovementComponent::CalculateGroundTrace() {
	FHitResult outputHit;

	FVector ownerUpVector = GetOwner()->GetActorUpVector();
	FVector groundTraceEndPoint = GetOwner()->GetActorLocation() + (ownerUpVector * -groundTraceDistance);

	GetWorld()->LineTraceSingleByChannel(outputHit, GetOwner()->GetActorLocation(), groundTraceEndPoint, ECC_WorldDynamic, groundTraceParams);
	groundTrace = outputHit;
	return outputHit;
}

FRotator UVelocityMovementComponent::GetGroundAlignRotator() const {
	if (groundTrace.IsValidBlockingHit())
		return UKismetMathLibrary::MakeRotFromZX(groundTrace.Normal, GetOwner()->GetActorForwardVector());
	else
		return GetOwner()->GetActorRotation();
}

void UVelocityMovementComponent::SetPositionZ(float NewZ) {
	FVector ownerLocation = GetOwner()->GetActorLocation();
	GetOwner()->SetActorLocation(FVector(ownerLocation.X, ownerLocation.Y, NewZ), true);
}

void UVelocityMovementComponent::Move(FVector DeltaLocation) {
	FHitResult moveHit;
	bool groundedMove = false;

	if (GetGroundTrace().IsValidBlockingHit() && GetGravity() >= 0) {
		groundedMove = true;
		SafeMoveUpdatedComponent(FVector::UpVector * 25.0f, GetOwner()->GetActorRotation(), true, moveHit);
	}

	SafeMoveUpdatedComponent(DeltaLocation, GetOwner()->GetActorRotation(), true, moveHit, ETeleportType::TeleportPhysics);
	if (moveHit.IsValidBlockingHit())
		SlideAlongSurface(DeltaLocation, 1.0f - moveHit.Time, moveHit.Normal, moveHit, false);

	if (groundedMove)
		SafeMoveUpdatedComponent(-FVector::UpVector * 25.0f, GetOwner()->GetActorRotation(), true, moveHit);
}

void UVelocityMovementComponent::SetActiveMover(UMover* mover) { 
	if (mover == activeMover)
		return;

	if (activeMover != nullptr) {
		if (mover->GetPriority() < activeMover->GetPriority()) {
			if (mover->IsQueueable())
				queuedMover = mover;
			return;
		}

		activeMover->OnDeactivate();
	}

	if (mover->IsQueueable())
		queuedMover = mover;

	activeMover = mover; 
	activeMover->OnActivate(desiredMovement);
}

void UVelocityMovementComponent::UseQueuedMover() {
	if (queuedMover == nullptr)
		return;

	if (activeMover != nullptr)
		activeMover->OnDeactivate();

	activeMover = queuedMover;
	queuedMover->OnActivate(desiredMovement);
	queuedMover = activeMover;
}

void UVelocityMovementComponent::OnEnterGround(FHitResult GroundTrace, FVector EntryVector) {
	// Run the OnEnterGround function of the Mover and return if the function disables sliding
	if (activeMover != nullptr)
		if (!activeMover->OnEnterGround()) return;

	FVector slideVector = ComputeSlideVector(FVector(0.0f, 0.0f, EntryVector.Z), 1.0f - GroundTrace.Time, GroundTrace.Normal, GroundTrace);
	AddVelocity(FVector(slideVector.X, slideVector.Y, 0.0f).GetSafeNormal() * slideVector.Size());
};

void UVelocityMovementComponent::OnExitGround() {
	// Run the OnExitGround function of the Mover and return if the function disables velocity transfer
	if (activeMover != nullptr)
		if (!activeMover->OnExitGround()) return;

	velocity = trueVelocity;
}

void UVelocityMovementComponent::SetDesiredMovement(FVector DesiredMovement) {
	if (DesiredMovement.Size() <= 0.15f) {
		desiredMovement = FVector::ZeroVector;
		return;
	}
	desiredMovement = DesiredMovement.GetClampedToMaxSize(1.0f);
}

void UVelocityMovementComponent::SetFootLocation(FVector Location) {
	GetOwner()->SetActorLocation(Location + FVector(0.0f, 0.0f, halfHeight));
}