// Copyright Epic Games, Inc. All Rights Reserved.


#include "RogueSkyGameModeBase.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

ARogueSkyGameModeBase::ARogueSkyGameModeBase() {
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void ARogueSkyGameModeBase::BeginPlay() {
	windUpdateDynamic = UMaterialInstanceDynamic::Create(windUpdateMaterial, this);
	windCopyToBufferDynamic = UMaterialInstanceDynamic::Create(windCopyToBufferMaterial, this);

	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), windField);
	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), windBuffer);

	windUpdateDynamic->SetTextureParameterValue("Wind Buffer", windBuffer);
	windCopyToBufferDynamic->SetTextureParameterValue("Texture to Copy", windField);
}

void ARogueSkyGameModeBase::PreInitializeComponents() {
	Super::PreInitializeComponents();
	
	chunkManager = Cast<AChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AChunkManager::StaticClass()));
}

// Called every frame
void ARogueSkyGameModeBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	UKismetRenderingLibrary::DrawMaterialToRenderTarget(GetWorld(), windField, windUpdateDynamic);
	UKismetRenderingLibrary::DrawMaterialToRenderTarget(GetWorld(), windBuffer, windCopyToBufferDynamic);
}

void ARogueSkyGameModeBase::SetWindOrigin(FVector Location, float Size) {
	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), windField);
	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), windBuffer);
}
