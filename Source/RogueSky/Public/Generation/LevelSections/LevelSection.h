// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Generation/DataTypes/DistanceField.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "LevelSection.generated.h"

UCLASS(Abstract)
class ROGUESKY_API ALevelSection : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelSection();

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
		UHierarchicalInstancedStaticMeshComponent* instancedStaticMesh;

	UPROPERTY(EditAnywhere)
		float spreadSpacing = 100.0f;
	TMap<TTuple<int16, int16>, bool> spreadPoints;
	uint16 activatedSpreadCount = 0;

protected:
	UPROPERTY(EditAnywhere)
		float minRadius = 8192.0f;
	UPROPERTY(EditAnywhere)
		float maxRadius = 16384.0f;

	TMap<FIntVector8, DistanceField*> distanceFields;
	USphereComponent* sectionArea;

public:
	UFUNCTION(BlueprintPure)
		FVector2D GetLocation2D() const { return FVector2D(GetActorLocation()); }

	void CreateSpreadPoints();
	UFUNCTION(BlueprintCallable)
		bool ActivateSpreadPoint(int X, int Y);
	UFUNCTION(BlueprintPure)
		float GetSpreadSpacing() const { return spreadSpacing; }
	TTuple<int16, int16> GetLocationTuple(int16 X, int16 Y) const;

	virtual void Generate() {};
	UFUNCTION(BlueprintPure)
		virtual bool LocationIsInside(FVector2D Location, float DistanceToEdge = 0.0f) const { return false; }
	UFUNCTION(BlueprintPure)
		virtual FVector GetLocationOnSurface(FVector2D Location) const { return FVector::ZeroVector; }
	UFUNCTION(BlueprintPure)
		virtual FVector GetSurfaceNormal(FVector2D Point) const { return FVector::ZeroVector; }
	UFUNCTION(BlueprintPure)
		virtual float GetEdgeDistanceFromOrigin(FVector2D EdgeDirection) const { return 0.0f; }
	UFUNCTION(BlueprintPure)
		FVector GetSurfaceOrigin() const { return GetLocationOnSurface(GetLocation2D()); }
	UFUNCTION(BlueprintPure)
		float GetRadius() const { return sectionArea->GetUnscaledSphereRadius(); }

	const TMap<FIntVector8, DistanceField*>& GetDistanceFields() const { return distanceFields; }
};
