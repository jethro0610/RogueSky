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

void UAllocator::CreateDelauneyGraph() {
    if (delauneyGraph != nullptr)
        delauneyGraph->MarkAsGarbage();

    TArray<FAllocatorTriangle> triangles;

    UAllocatorNode* superNodes[3];
    superNodes[0] = NewObject<UAllocatorNode>();
    superNodes[0]->Initialize(FVector2D(-65536, -65536), 0.0f, 0.0f);

    superNodes[1] = NewObject<UAllocatorNode>();
    superNodes[1]->Initialize(FVector2D(0, 65536), 0.0f, 0.0f);

    superNodes[2] = NewObject<UAllocatorNode>();
    superNodes[2]->Initialize(FVector2D(65536, -65536), 0.0f, 0.0f);
    FAllocatorTriangle superTriangle = FAllocatorTriangle(superNodes[0], superNodes[1], superNodes[2]);

    triangles.Add(superTriangle);

    for (UAllocatorNode* node : nodes) {
        TArray<FAllocatorTriangle> trianglesToRemove;
        for (FAllocatorTriangle triangle : triangles) {
            FVector2D circleOrigin;
            float circleRadius;
            triangle.GetCircumcircle(circleOrigin, circleRadius);

            if (FVector2D::Distance(circleOrigin, node->GetLocation()) <= circleRadius)
                trianglesToRemove.Add(triangle);
        }

        TArray<FAllocatorEdge> polygon;
        for (FAllocatorTriangle badTriangle : trianglesToRemove) {
            for (FAllocatorEdge edge : badTriangle.edges) {
                bool shouldAddEdge = true;
                for (FAllocatorTriangle other : trianglesToRemove) {
                    if (badTriangle == other)
                        continue;

                    if (other.HasEdge(edge))
                        shouldAddEdge = false;
                }

                if (shouldAddEdge)
                    polygon.Add(edge);
            }
            triangles.Remove(badTriangle);
        }

        for (FAllocatorEdge edge : polygon) {
            FAllocatorTriangle newTriangle = FAllocatorTriangle::CreateTriangleFromNodeAndEdge(node, edge);
            triangles.Add(newTriangle);
        }
    }

    TSet<FAllocatorTriangle> trianglesWithSuperNode;
    for (FAllocatorTriangle triangle : triangles)
    for (FAllocatorEdge edge : triangle.edges) 
    for (UAllocatorNode* node : edge.nodes) {
        for (UAllocatorNode* superNode : superNodes) {
            if (node == superNode)
                trianglesWithSuperNode.Add(triangle);
        }
    }

    for (FAllocatorTriangle triangle : trianglesWithSuperNode)
        triangles.Remove(triangle);

    delauneyGraph = NewObject<UAllocatorGraph>(this);
    for (FAllocatorTriangle triangle : triangles)
    for (FAllocatorEdge edge : triangle.edges) {
        delauneyGraph->AddEdge(edge);
    }

    superNodes[0]->MarkAsGarbage();
    superNodes[1]->MarkAsGarbage();
    superNodes[2]->MarkAsGarbage();
}