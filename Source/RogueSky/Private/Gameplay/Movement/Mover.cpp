#include "Gameplay/Movement/Mover.h"
#include "Gameplay/Movement/VelocityMovementComponent.h"

void UMover::BeginPlay() {
    Super::BeginPlay();
    movementComponent = GetOwner()->FindComponentByClass<UVelocityMovementComponent>();
}