// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "EnviornmentPerceptionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPercievePlayer, AActor*, PercievedActor);

UCLASS(meta = (BlueprintSpawnableComponent))
class ROGUESKY_API UEnviornmentPerciever : public USphereComponent
{
	GENERATED_BODY()
	
public:
	UEnviornmentPerciever();

public:
	UPROPERTY(EditAnywhere)
		float maxPerceptionAngle = 45.0f;

private:
	TSet<TWeakObjectPtr<AActor>> actorsInPerceptionDistance;
	TSet<TWeakObjectPtr<AActor>> percievedActors;

	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void BeginPlay() override;
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlap(class UPrimitiveComponent* OverlapComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

public:
	UPROPERTY(BlueprintAssignable)
		FOnPercievePlayer OnPercievePlayer;
};
