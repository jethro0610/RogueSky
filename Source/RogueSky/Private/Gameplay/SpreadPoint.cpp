// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/SpreadPoint.h"
#include "Gameplay/SpreadManager.h"

// Sets default values
ASpreadPoint::ASpreadPoint() {
    collider = CreateDefaultSubobject<USphereComponent>("Collider");
    collider->SetSphereRadius(32.0f, false);
    collider->SetCollisionProfileName("SpreadPoint");
    SetRootComponent(collider);
}

// Called when the game starts or when spawned
void ASpreadPoint::BeginPlay() {
	Super::BeginPlay();
	index = ASpreadManager::GetSpreadManager()->CreateSpreadIndex();
    collider->OnComponentBeginOverlap.AddDynamic(this, &ASpreadPoint::OnOverlap);
}


void ASpreadPoint::OnOverlap(UPrimitiveComponent* OverlappedComponent,
AActor* OtherActor,
UPrimitiveComponent* OtherComp,
int32 OtherBodyIndex,
bool bFromSweep,
const FHitResult& SweepResult) {
    ASpreadManager::GetSpreadManager()->ActivateSpreadIndex(index, GetActorTransform());
}
