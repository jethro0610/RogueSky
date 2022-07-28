// Fill out your copyright notice in the Description page of Project Settings.
#include "Gameplay/Interaction/ElasticFieldInfluencer.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UElasticFieldInfluencer::UElasticFieldInfluencer() {
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UElasticFieldInfluencer::BeginPlay() {
	Super::BeginPlay();

	dynamicInfluenceMaterial = UMaterialInstanceDynamic::Create(influenceMaterial, this);
	gamemode = Cast<ARogueSkyGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	dynamicInfluenceMaterial->SetScalarParameterValue("Radius", influenceRadius / gamemode->GetElasticFieldSize());
}


// Called every frame
void UElasticFieldInfluencer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	bool isInField;
	FVector2D positionInField;
	gamemode->GetPositionInElasticField(FVector2D(GetOwner()->GetActorLocation()), isInField, positionInField);

	if (isInField) {
		dynamicInfluenceMaterial->SetVectorParameterValue("Location", FVector4(positionInField.X, positionInField.Y, 0.0f, 0.0f));
		UKismetRenderingLibrary::DrawMaterialToRenderTarget(GetWorld(), gamemode->GetPositionBuffer(), dynamicInfluenceMaterial);
	}
}

void UElasticFieldInfluencer::SetRadius(float Radius) {
	influenceRadius = Radius;
	dynamicInfluenceMaterial->SetScalarParameterValue("Radius", influenceRadius / gamemode->GetElasticFieldSize());
}

