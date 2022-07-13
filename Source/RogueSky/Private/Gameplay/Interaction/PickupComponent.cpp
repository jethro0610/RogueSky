// Fill out your copyright notice in the Description page of Project Settings.
#include "Gameplay/Interaction/PickupComponent.h"
#include "Kismet/KismetMathLibrary.h"

UPickupComponent::UPickupComponent() {
    SetCollisionObjectType(ECC_GameTraceChannel14);
    SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    PrimaryComponentTick.bCanEverTick = true;
}

void UPickupComponent::BeginPlay() {
    Super::BeginPlay();
    OnComponentBeginOverlap.AddDynamic(this, &UPickupComponent::OnBeginOverlap);
    OnComponentEndOverlap.AddDynamic(this, &UPickupComponent::OnEndOverlap);
    targeter = GetOwner()->FindComponentByClass<UTargeterComponent>();
}

void UPickupComponent::ActivateHoldSlot(int HoldSlot) {
    if (heldActors[HoldSlot].Num() != 0)
        StartUsing(HoldSlot);
    else
        StartPickingUp(HoldSlot);
}

void UPickupComponent::StartPickingUp(int HoldSlot) {
    if (IsPickingUp() || IsUsing())
        return;

    OnStartPickup.Broadcast();
    pickupHoldSlot = HoldSlot;

    TArray<TWeakObjectPtr<AActor>> actors = overlappedActors.Array();
    for (TWeakObjectPtr<AActor> actor : actors)
        PickupActor(actor.Get(), pickupHoldSlot);
}

void UPickupComponent::PickupActor(AActor* ActorToPickup, int HoldSlot) {
    if (heldActors[HoldSlot].Contains(ActorToPickup))
        return;

    if (!ActorToPickup->Implements<UPickupInterface>())
        return;

    IPickupInterface* pickupInterface = Cast<IPickupInterface>(ActorToPickup);

    if (pickupInterface->Execute_CanPickup(ActorToPickup)) {
        pickupInterface->Execute_OnPickup(ActorToPickup, GetOwner()->GetRootComponent());
        ActorToPickup->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
    }
    heldActors[HoldSlot].Add(ActorToPickup);
}

void UPickupComponent::StopPickingUp() {
    pickupHoldSlot = -1;
}

void UPickupComponent::StartUsing(int HoldSlot) {
    if (IsPickingUp())
        return;

    if (IsUsing() && !bUseInterruptable)
        return;

    if (bUseImmediatly)
        UseHoldSlot(HoldSlot);
    else if (useCombo < maxUseCombo){
        useCombo++;
        holdSlotToUse = HoldSlot;
        bUseInterruptable = false;
        OnStartUse.Broadcast();
    }
}

void UPickupComponent::UseHoldSlot(int HoldSlot) {
    // Assign the HoldSlot to holdSlotToUse is it's unassigned...
    if (HoldSlot == -1)
        HoldSlot = holdSlotToUse;

    // If the newly assigned value is still -1, then return
    if (HoldSlot == -1)
        return;

    AActor* useActor = heldActors[HoldSlot].Pop().Get();
    useActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    IPickupInterface* pickupInterface = Cast<IPickupInterface>(useActor);
    USceneComponent* target = targeter == nullptr ? nullptr : targeter->GetTarget();
    pickupInterface->Execute_OnUse(useActor, target, GetOwner()->GetActorForwardVector());
}

void UPickupComponent::SetUseInterruptable(bool Interruptable) {
    bUseInterruptable = Interruptable;
}

void UPickupComponent::StopUsing() {
    holdSlotToUse = -1;
    useCombo = 0;
    OnStopUse.Broadcast();
}

FHoldSlotInfo UPickupComponent::GetHoldSlotInfo(int HoldSlot) const {
    FHoldSlotInfo returnInfo;
    returnInfo.amount = heldActors[HoldSlot].Num();
    if (returnInfo.amount == 0)
        return returnInfo;

    // Get the name of the first actor in the hold slot
    AActor* firstActorInSlot = heldActors[HoldSlot][0].Get();
    IPickupInterface* pickupInterface = Cast<IPickupInterface>(firstActorInSlot);
    returnInfo.itemName = pickupInterface->Execute_GetPickupName(firstActorInSlot);

    return returnInfo;
}

int UPickupComponent::GetHoldSlotAmount(int HoldSlot) const {
    return heldActors[HoldSlot].Num();
}

FName UPickupComponent::GetHoldSlotItemName(int HoldSlot) const {
    if (heldActors[HoldSlot].Num() == 0)
        return "";

    // Get the name of the first actor in the hold slot
    AActor* firstActorInSlot = heldActors[HoldSlot][0].Get();
    IPickupInterface* pickupInterface = Cast<IPickupInterface>(firstActorInSlot);
    return pickupInterface->Execute_GetPickupName(firstActorInSlot);
}

void UPickupComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
    
}

void UPickupComponent::OnBeginOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult) {
    if (!OtherActor->Implements<UPickupInterface>())
        return;

    if (overlappedActors.Contains(OtherActor))
        return;

    OtherActor->OnDestroyed.AddUniqueDynamic(this, &UPickupComponent::OnActorDestroyed);

    overlappedActors.Add(OtherActor);
    if (IsPickingUp())
        PickupActor(OtherActor, pickupHoldSlot);
}

void UPickupComponent::OnEndOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) {
    for (int i = 0; i < 4; i++) {
        if (heldActors[i].Contains(OtherActor))
            return;
    }

    OtherActor->OnDestroyed.RemoveDynamic(this, &UPickupComponent::OnActorDestroyed);
    overlappedActors.Remove(OtherActor);
}

void UPickupComponent::OnActorDestroyed(AActor* DestroyedActor) {
    overlappedActors.Remove(DestroyedActor);
    // POSSIBLE BUG: An actor may be destroyed while being held. Either prevent destruction or remove from held items when deleted
}