// Fill out your copyright notice in the Description page of Project Settings.
#include "Gameplay/Interaction/SpreadActivator.h"

// Sets default values for this component's properties
USpreadActivator::USpreadActivator() {
	PrimaryComponentTick.bCanEverTick = true;
	
}


// Called when the game starts
void USpreadActivator::BeginPlay() {
	Super::BeginPlay();
	Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent())->OnComponentBeginOverlap.AddDynamic(this, &USpreadActivator::OnOverlap);
}


// Called every frame
void USpreadActivator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (currentSection == nullptr)
		return;

	FVector2D offsetVector = FVector2D(GetOwner()->GetActorLocation()) - currentSection->GetLocation2D();
	offsetVector /= currentSection->GetSpreadSpacing();

	int numActivated = 0;
	for (int x = -radius; x <= radius; x++)
	for (int y = -radius; y <= radius; y++) {
		if (x * x + y * y > radius * radius)
			continue;
		
		if (currentSection->ActivateSpreadPoint((int)offsetVector.X + x, (int)offsetVector.Y + y))
			numActivated++;
	}

	if (numActivated > 0)
		OnActivateSpread.Broadcast(numActivated);
}

void USpreadActivator::OnOverlap(UPrimitiveComponent* OverlappedComp, 
AActor* OtherActor,
UPrimitiveComponent* OtherComp, 
int32 OtherBodyIndex, 
bool bFromSweep, 
const FHitResult& SweepResult) {
	if (OtherComp->GetCollisionProfileName() != "LevelSection")
		return;

	currentSection = Cast<ALevelSection>(OtherActor);
}

