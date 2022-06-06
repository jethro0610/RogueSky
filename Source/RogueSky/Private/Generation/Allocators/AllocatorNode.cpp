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

void FAllocatorEdge::GetStandardForm(float& OutA, float& OutB, float& OutC) const {
    OutA = nodes[1]->GetLocation().Y - nodes[0]->GetLocation().Y;
    OutB = nodes[0]->GetLocation().X - nodes[1]->GetLocation().X;
    OutC = OutA * nodes[0]->GetLocation().X + OutB * nodes[0]->GetLocation().Y;
}

FVector2D FAllocatorEdge::GetMidpoint() const {
    return (nodes[0]->GetLocation() + nodes[1]->GetLocation()) / 2.0f;
}

void FAllocatorEdge::GetPerpindicularBisector(float& OutA, float& OutB, float& OutC) const {
    float a, b, c;
    FVector2D midpoint = GetMidpoint();
    GetStandardForm(a, b, c);

    OutC = -b * midpoint.X + a * midpoint.Y;

    OutA = -b;
    OutB = a;
}

bool FAllocatorTriangle::HasEdge(FAllocatorEdge Edge) const {
    for (FAllocatorEdge other : edges) {
        if (Edge == other)
            return true;
    }
    return false;
}

void FAllocatorTriangle::GetCircumcircle(FVector2D& Origin, float& Radius) const {
    float a1, b1, c1; 
    edges[0].GetPerpindicularBisector(a1, b1, c1);

    float a2, b2, c2;
    edges[1].GetPerpindicularBisector(a2, b2, c2);

    float determinant = a1 * b2 - a2 * b1;
    if (determinant == 0.0f) {
        Origin = FVector2D::ZeroVector;
        Radius = 1.0f;
        return;
    }
    else {
        Origin.X = (b2 * c1 - b1 * c2) / determinant;
        Origin.Y = (a1 * c2 - a2 * c1) / determinant;
        Radius = FVector2D::Distance(Origin, edges[0].nodes[0]->GetLocation());
    }
}

FAllocatorTriangle FAllocatorTriangle::CreateTriangleFromNodeAndEdge(UAllocatorNode* Node, FAllocatorEdge Edge) {
    FAllocatorEdge newEdge1 = FAllocatorEdge(Node, Edge.nodes[0]);
    FAllocatorEdge newEdge2 = FAllocatorEdge(Node, Edge.nodes[1]);
    return FAllocatorTriangle(Edge, newEdge1, newEdge2);
}