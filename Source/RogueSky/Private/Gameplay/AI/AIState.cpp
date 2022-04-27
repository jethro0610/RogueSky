// Fill out your copyright notice in the Description page of Project Settings.
#include "Gameplay/AI/AIState.h"

void UAIState::BeginPlay() {
    Super::BeginPlay();
    stateManager = GetOwner()->FindComponentByClass<UAIStateManager>();
    targeterComponent = GetOwner()->FindComponentByClass<UTargeterComponent>();
}

USceneComponent* UAIState::GetTarget() const {
	if (targeterComponent == nullptr)
		return nullptr;

	return targeterComponent->GetTarget();
}

void UAIState::EnterState_Implementation() {

}

void UAIState::EndState() {
    stateManager->SetState(nullptr);
    OnEndState.Broadcast();
}

void UAIState::ExitState_Implementation() {

}