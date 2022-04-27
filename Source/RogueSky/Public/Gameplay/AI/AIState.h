// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Gameplay/AI/AIStateManager.h"
#include "Gameplay/TargeterComponent.h"
#include "AIState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnterAIState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndAIState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExitAIState);

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class ROGUESKY_API UAIState : public UActorComponent
{
	GENERATED_BODY()

private:
	virtual void BeginPlay() override;

public:
	friend UAIStateManager;

	UFUNCTION(BlueprintNativeEvent)
		FVector TickState(float DeltaTime);
	FVector TickState_Implementation(float DeltaTime) { return FVector::ZeroVector; }

	UFUNCTION(BlueprintNativeEvent)
		void EnterState();
	virtual void EnterState_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void ExitState();
	virtual void ExitState_Implementation();

	UFUNCTION(BlueprintCallable)
		void EndState();

	UPROPERTY(BlueprintAssignable)
		FOnEnterAIState OnEnterState;

	UPROPERTY(BlueprintAssignable)
		FOnEndAIState OnEndState;

	UPROPERTY(BlueprintAssignable)
		FOnEnterAIState OnExitState;

	UFUNCTION(BlueprintPure)
		UAIStateManager* GetStateManager() const { return stateManager; }

	UFUNCTION(BlueprintPure)
		USceneComponent* GetTarget() const;

private:
	UAIStateManager* stateManager;
	UTargeterComponent* targeterComponent;
};
