// Fill out your copyright notice in the Description page of Project Settings.
#include "Generation/Allocators/Allocator.h"
#include "Math/UnrealMathUtility.h"

void UAllocator::PlaceRandomNodes(int NumberOfNodes) {
    for (int i = 0; i < NumberOfNodes; i++) {
        FVector2D location = FVector2D(FMath::RandRange(-placementRange, placementRange), FMath::RandRange(-placementRange, placementRange));
        float radius = FMath::RandRange(minRadius, maxRadius);
        float distance = FMath::RandRange(minDist, maxDist);

        UAllocatorNode* newNode = NewObject<UAllocatorNode>(this);
        newNode->Initialize(location, radius, distance);
        nodes.Add(newNode);
    }
}

void UAllocator::AllocateNodes() {
    bool resolvedAll = false;

    while (!resolvedAll) {
        resolvedAll = true;
        for (UAllocatorNode* node : nodes) {
            for (UAllocatorNode* other : nodes) {
                if (other == node)
                    continue;

                if (node->SolveCollision(other) == true)
                    resolvedAll = false;
            }
        }
    }
}