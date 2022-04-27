// Fill out your copyright notice in the Description page of Project Settings.
#include "Gameplay/AI/AIStateManager.h"
#include "Gameplay/AI/AIState.h"

UAIStateManager::UAIStateManager(){
	PrimaryComponentTick.bCanEverTick = true;
}

void UAIStateManager::BeginPlay() {
	movementComponent = GetOwner()->FindComponentByClass<UVelocityMovementComponent>();
}

void UAIStateManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FVector stateOutput = FVector::ZeroVector;
	if (currentState != nullptr)
		stateOutput = currentState->TickState(DeltaTime);

	if (movementComponent != nullptr)
		movementComponent->SetDesiredMovement(stateOutput);
}

void UAIStateManager::SetState(UAIState* State) {
	if (State == currentState)
		return;

	if (currentState != nullptr) {
		currentState->ExitState();
		currentState->OnExitState.Broadcast();
	}

	if (enumState != 255) {
		OnExitEnumState.Broadcast(enumState);
		enumState = 255;
	}

	if (State != nullptr) {
		State->EnterState();
		State->OnEnterState.Broadcast();
		stateStack.Push(TPair<UAIState*, uint8>(State, 255));

		if (stateStack.Num() > 4)
			stateStack.RemoveAt(0);
	}

	currentState = State;
}

void UAIStateManager::SetEnumState(uint8 EnumState) {
	if (EnumState == enumState)
		return;

	if (currentState != nullptr) {
		currentState->ExitState();
		currentState->OnExitState.Broadcast();
		currentState = nullptr;
		movementComponent->SetDesiredMovement(FVector::ZeroVector);
	}

	if (enumState != 255)
		OnExitEnumState.Broadcast(enumState);

	if (EnumState != 255) {
		OnEnterEnumState.Broadcast(EnumState);
		stateStack.Push(TPair<UAIState*, uint8>(nullptr, EnumState));

		if (stateStack.Num() > 4)
			stateStack.RemoveAt(0);
	}

	enumState = EnumState;
}

void UAIStateManager::GetState(UAIState*& CurrentState, uint8& EnumState, EStateOutput& StateOutput) {
	if (enumState != 255) {
		StateOutput = EStateOutput::StateEnum;
		EnumState = enumState;
	}
	else {
		StateOutput = EStateOutput::StateClass;
		CurrentState = currentState;
	}
}

void UAIStateManager::GoToPreviousState(int StatesToGoBackBy) {
	// Pop out any states that come after the state were going back to
	for (int i = 0; i < StatesToGoBackBy; i++)
		stateStack.Pop();

	TPair<UAIState*, uint8> stateToGoBackTo = stateStack.Pop();
	if (stateToGoBackTo.Value == 255) // Check if an enum state was used here
		SetState(stateToGoBackTo.Key);
	else
		SetEnumState(stateToGoBackTo.Value);
}

