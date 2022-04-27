// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Gameplay/VelocityMovementComponent.h"
#include "Gameplay/TargeterComponent.h"
#include "AIStateManager.generated.h"

class UAIState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnterEnumState, uint8, EnumState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnExitEnumState, uint8, EnumState);

UENUM(BlueprintType)
enum class EStateOutput : uint8 {
	StateClass,
	StateEnum
};

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class ROGUESKY_API UAIStateManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAIStateManager();

private:
	void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable)
		void SetState(UAIState* State);

	UFUNCTION(BlueprintCallable, Meta = (ExpandEnumAsExecs = "StateOutput"))
		void GetState(UAIState*& CurrentState, uint8& EnumState, EStateOutput& StateOutput);

	UFUNCTION(BlueprintPure)
		UAIState* GetAIState() const { return currentState; }

	UFUNCTION(BlueprintPure)
		uint8 GetEnumState() const { return enumState; }

	UFUNCTION(BlueprintCallable)
		void SetEnumState(uint8 EnumState);

	UFUNCTION(BlueprintCallable)
		void GoToPreviousState(int StatesGoBackBy = 1);

	UPROPERTY(BlueprintAssignable)
		FOnEnterEnumState OnEnterEnumState;

	UPROPERTY(BlueprintAssignable)
		FOnExitEnumState OnExitEnumState;

private:
	UVelocityMovementComponent* movementComponent;
	UAIState* currentState;
	uint8 enumState = 255;
	TArray<TPair<UAIState*, uint8>> stateStack;
};
