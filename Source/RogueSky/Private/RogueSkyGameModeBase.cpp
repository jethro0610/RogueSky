// Copyright Epic Games, Inc. All Rights Reserved.


#include "RogueSkyGameModeBase.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

ARogueSkyGameModeBase::ARogueSkyGameModeBase() {
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void ARogueSkyGameModeBase::BeginPlay() {
	updateVelocityDynamic = UMaterialInstanceDynamic::Create(updateVelocityMaterial, this);
	updatePositionDynamic = UMaterialInstanceDynamic::Create(updatePositionMaterial, this);
	copyToBufferDynamic = UMaterialInstanceDynamic::Create(copyToBufferMaterial, this);

	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), currentVelocityRT);
	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), velocityBufferRT);
	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), currentPositionRT);
	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), positionBufferRT);

	updateVelocityDynamic->SetTextureParameterValue("Velocity Buffer", velocityBufferRT);
	updateVelocityDynamic->SetTextureParameterValue("Position Buffer", positionBufferRT);

	updatePositionDynamic->SetTextureParameterValue("Velocity Buffer", velocityBufferRT);
	updatePositionDynamic->SetTextureParameterValue("Position Buffer", positionBufferRT);
}

void ARogueSkyGameModeBase::PreInitializeComponents() {
	Super::PreInitializeComponents();
	
	chunkManager = Cast<AChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AChunkManager::StaticClass()));
}

// Called every frame
void ARogueSkyGameModeBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	UKismetRenderingLibrary::DrawMaterialToRenderTarget(GetWorld(), currentVelocityRT, updateVelocityDynamic);
	copyToBufferDynamic->SetTextureParameterValue("Texture to Copy", currentVelocityRT);
	UKismetRenderingLibrary::DrawMaterialToRenderTarget(GetWorld(), velocityBufferRT, copyToBufferDynamic);

	UKismetRenderingLibrary::DrawMaterialToRenderTarget(GetWorld(), currentPositionRT, updatePositionDynamic);
	copyToBufferDynamic->SetTextureParameterValue("Texture to Copy", currentPositionRT);
	UKismetRenderingLibrary::DrawMaterialToRenderTarget(GetWorld(), positionBufferRT, copyToBufferDynamic);
}

void ARogueSkyGameModeBase::SetRTFieldOrigin(FVector2D Location, float Size) {
	rtFieldOrigin = Location;
	rtFieldSize = Size;
	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), currentVelocityRT);
	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), velocityBufferRT);
	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), currentPositionRT);
	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), positionBufferRT);
}

FVector2D ARogueSkyGameModeBase::GetPositionInRTField(FVector2D Location, bool& IsInField) const {
	FVector2D rtPosition =  ((Location - rtFieldOrigin) / rtFieldSize) + FVector2D(0.5f, 0.5f);

	if (rtPosition.X < 0.0f || rtPosition.X > 1.0f || rtPosition.Y < 0.0f || rtPosition.Y > 1.0f) {
		IsInField = false;
		return FVector2D::ZeroVector;
	}

	IsInField = true;
	return rtPosition;
}
