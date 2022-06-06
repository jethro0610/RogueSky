// Fill out your copyright notice in the Description page of Project Settings.
#include "Generation/Allocators/AllocatorNode.h"

bool UAllocatorNode::SolveCollision(UAllocatorNode* Other) {
    float greatestMinimumDistance = FMath::Max(minDistance, Other->minDistance);
    float distanceForCollision = radius + Other->radius + greatestMinimumDistance;
    float distanceBetweenNodes = FVector2D::Distance(location, Other->location);

    // If the nodes aren't touching, there was no collision
    // 5.0f is added to give leniency to the collision and prevent infinite loops
    if (distanceBetweenNodes >= distanceForCollision)
        return false;

    // Move both nodes out of collision equally from the midpoint of the nodes
    // this should be half the value for distance for collision
    FVector2D slope = (Other->location - location).GetSafeNormal();
    FVector2D midPoint = (location + Other->location) / 2.0f;
    location = midPoint + slope * distanceForCollision * 0.5f;
    Other->location = midPoint - slope * distanceForCollision * 0.5f;

    return true;
}