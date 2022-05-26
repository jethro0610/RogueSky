// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Rail.generated.h"

UCLASS()
class ROGUESKY_API ARail : public AActor {
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARail();

	void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
		USceneComponent* sceneComponent;

	UPROPERTY(VisibleAnywhere)
		USplineComponent* spline;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* collider;

	UPROPERTY(EditAnywhere)
		float railRadius = 150.0f;

	UPROPERTY(EditAnywhere)
		float colliderSectionLength = 75.0f;

public:
	UFUNCTION(BlueprintPure)
		void LocationIsTouchingRail(FVector Location, bool& IsTouching, FTransform& TransformOnRail) const;

	UFUNCTION(BlueprintPure)
		FTransform GetTransformAlongRail(FVector Location) const;

	UFUNCTION(BlueprintPure)
		FVector GetStartpoint() const;

	UFUNCTION(BlueprintPure)
		FVector GetEndpoint() const;
};
