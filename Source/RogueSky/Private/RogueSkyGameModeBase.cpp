// Copyright Epic Games, Inc. All Rights Reserved.


#include "RogueSkyGameModeBase.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

ARogueSkyGameModeBase::ARogueSkyGameModeBase() {
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void ARogueSkyGameModeBase::StartPlay() {
	Super::StartPlay();
	
	chunkManager = Cast<AChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AChunkManager::StaticClass()));
	spreadManager = Cast<ASpreadManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ASpreadManager::StaticClass()));
}

// Called every frame
void ARogueSkyGameModeBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
