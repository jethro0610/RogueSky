// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mover.h"
#include "Gameplay/Level/Rail.h"
#include "RailMover.generated.h"

UCLASS(meta = (BlueprintSpawnableComponent))
class ROGUESKY_API URailMover : public UMover {
	GENERATED_BODY()
	
public:
    URailMover();
	void BeginPlay() override;

protected:
    void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UPROPERTY(EditAnywhere)
        float maxSpeed = 20.0f;
    UPROPERTY(EditAnywhere)
        float cooldownLength = 0.5f;
    UPROPERTY(EditAnywhere)
        float accelerationSpeed = 0.15f;
    UPROPERTY(EditAnywhere)
        float breakSpeed = 0.5f;

	ARail* rail;
    FTransform railTransform;
    float currentSpeed = 0.0f;
    float cooldown = 0.0f;

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

    void StartRailMovement(ARail* RailToMoveOn, FTransform RailTransform);
    void OnActivate_Implementation(FVector DesiredMovement) override;
    void OnDeactivate_Implementation() override;

    void DoMovement_Implementation(float DeltaTime, FVector DesiredMovement) override;
};
