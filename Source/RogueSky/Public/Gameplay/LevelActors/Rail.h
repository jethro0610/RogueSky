// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Generation/Generators/IslandGenerator.h"
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
		UStaticMesh* mesh;

	TArray<USplineMeshComponent> meshes;

public:
	UFUNCTION(BlueprintCallable)
		void SetMesh(UStaticMesh* StaticMesh) { mesh = StaticMesh; }

	UFUNCTION(BlueprintPure)
		void LocationIsTouchingRail(FVector Location, bool& IsTouching, FTransform& TransformOnRail) const;

	UFUNCTION(BlueprintPure)
		FTransform GetTransformAlongRail(FVector Location) const;

	UFUNCTION(BlueprintPure)
		FVector GetStartpoint() const;

	UFUNCTION(BlueprintPure)
		FVector GetEndpoint() const;

	UFUNCTION(BlueprintCallable)
		void UpdateBounds();

	UFUNCTION(BlueprintCallable)
		void CreateMeshes();

	UFUNCTION(BlueprintCallable)
		void AddPoint(FVector Location);

	UFUNCTION(BlueprintCallable)
		void SetEndpointLocation(FVector Location);

	void ConnectIslands(UIslandGenerator* Island1, UIslandGenerator* Island2);
};
