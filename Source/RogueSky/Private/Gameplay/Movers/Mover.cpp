#include "Gameplay/Movers/Mover.h"
#include "Gameplay/VelocityMovementComponent.h"

void UMover::BeginPlay() {
    Super::BeginPlay();
    movementComponent = GetOwner()->FindComponentByClass<UVelocityMovementComponent>();
}