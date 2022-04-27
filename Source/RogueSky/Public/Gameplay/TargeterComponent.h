// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargeterComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoseTarget);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUESKY_API UTargeterComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTargeterComponent();

private:
	FVector vectorToTarget;

protected:
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
		TWeakObjectPtr<USceneComponent> target;

	virtual void SetTargetLost();

public:
	UFUNCTION(BlueprintPure)
		USceneComponent* GetTarget() const { return target.Get(); }

	UFUNCTION(BlueprintPure)
		virtual FVector GetVectorToTarget() const { return vectorToTarget; }

	UFUNCTION(BlueprintCallable)
		void SetVectorToTarget(FVector VectorToTarget) { vectorToTarget = VectorToTarget; }

	UFUNCTION(BlueprintCallable)
		void SetTarget(USceneComponent* NewTarget);

	UPROPERTY(BlueprintAssignable)
		FOnLoseTarget OnLoseTarget;
};
