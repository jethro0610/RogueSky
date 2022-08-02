// Copyright Epic Games, Inc. All Rights Reserved.
#include "RogueSkyGameModeBase.h"
#include "Generation/LevelSections/LevelSection.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

ARogueSkyGameModeBase::ARogueSkyGameModeBase() {
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void ARogueSkyGameModeBase::BeginPlay() {
	copyToBufferDynamic = UMaterialInstanceDynamic::Create(copyToBufferMaterial, this);

	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), currentVelocityRT);
	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), velocityBufferRT);
	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), currentPositionRT);
	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), positionBufferRT);
	elasticFieldProperties = GetWorld()->GetParameterCollectionInstance(elasticFieldCollection);
	elasticFieldProperties->SetScalarParameterValue("Size", elasticFieldSize);

	levelGenerator->GenerateLevel();
	chunkManager->OnDoneGenerating.AddDynamic(this, &ARogueSkyGameModeBase::OnDoneGenerating);
}

void ARogueSkyGameModeBase::PreInitializeComponents() {
	Super::PreInitializeComponents();
	
	chunkManager = Cast<AChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AChunkManager::StaticClass()));
	levelGenerator = Cast<ALevelGenerator>(UGameplayStatics::GetActorOfClass(GetWorld(), ALevelGenerator::StaticClass()));
}

// Called every frame
void ARogueSkyGameModeBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	UKismetRenderingLibrary::DrawMaterialToRenderTarget(GetWorld(), currentVelocityRT, updateVelocityMaterial);
	copyToBufferDynamic->SetTextureParameterValue("Texture to Copy", currentVelocityRT);
	UKismetRenderingLibrary::DrawMaterialToRenderTarget(GetWorld(), velocityBufferRT, copyToBufferDynamic);

	UKismetRenderingLibrary::DrawMaterialToRenderTarget(GetWorld(), currentPositionRT, updatePositionMaterial);
	copyToBufferDynamic->SetTextureParameterValue("Texture to Copy", currentPositionRT);
	UKismetRenderingLibrary::DrawMaterialToRenderTarget(GetWorld(), positionBufferRT, copyToBufferDynamic);
}

void ARogueSkyGameModeBase::SetElasticFieldOrigin(FVector2D Location) {
	elasticFieldOrigin = Location;
	elasticFieldProperties->SetVectorParameterValue("Origin", FVector4(Location.X, Location.Y, 0.0f, 0.0f));
	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), currentVelocityRT);
	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), velocityBufferRT);
	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), currentPositionRT);
	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), positionBufferRT);
}

void ARogueSkyGameModeBase::GetPositionInElasticField(FVector2D Location, bool& bIsInField, FVector2D& PositionInField) const {
	PositionInField =  ((Location - elasticFieldOrigin) / elasticFieldSize) + FVector2D(0.5f, 0.5f);

	if (PositionInField.X < 0.0f || PositionInField.X > 1.0f || PositionInField.Y < 0.0f || PositionInField.Y > 1.0f) {
		bIsInField = false;
		PositionInField = FVector2D::ZeroVector;
	}

	bIsInField = true;
}

void ARogueSkyGameModeBase::SetCurrentSection(ALevelSection* Section) {
	currentSection = Section;
	SetElasticFieldOrigin(currentSection->GetLocation2D());
}

void ARogueSkyGameModeBase::OnDoneGenerating() {
	SetCurrentSection(levelGenerator->GetSpawnSection());
	player = GetWorld()->SpawnActor<AActor>(playerClass, currentSection->GetSurfaceOrigin() + FVector(0.0f, 0.0f, 48.0f), FRotator::ZeroRotator);
	player->FindComponentByClass<UPrimitiveComponent>()->OnComponentBeginOverlap.AddDynamic(this, &ARogueSkyGameModeBase::OnPlayerOverlap);
}

void ARogueSkyGameModeBase::OnPlayerOverlap(UPrimitiveComponent* OverlappedComp,
AActor* OtherActor,
UPrimitiveComponent* OtherComp,
int32 OtherBodyIndex,
bool bFromSweep,
const FHitResult& SweepResult) {
	if (OtherComp->GetCollisionProfileName() == "LevelSection") {
		ALevelSection* section = Cast<ALevelSection>(OtherActor);
		SetCurrentSection(section);
	}
}