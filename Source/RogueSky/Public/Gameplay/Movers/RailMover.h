// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Movers/Mover.h"
#include "Gameplay/LevelActors/Rail.h"
#include "RailMover.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class ROGUESKY_API URailMover : public UMover {
	GENERATED_BODY()
	
public:
    URailMover();
	void BeginPlay() override;

protected:
    void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	ARail* rail;

    TSet<ARail*> railsInBounds;

    UFUNCTION()
    void OnOverlap(UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

    UFUNCTION()
    void EndOverlap(UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex);

    void StartRailMovement(ARail* RailToMoveOn, FVector MoveStartLocation);
    void OnActivate_Implementation(FVector DesiredMovement) override;
    void OnDeactivate_Implementation() override;
};
