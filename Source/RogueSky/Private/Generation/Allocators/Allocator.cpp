// Fill out your copyright notice in the Description page of Project Settings.
#include "Generation/Allocators/Allocator.h"
#include "Math/NumericLimits.h"
#include "Math/UnrealMathUtility.h"

void UAllocator::PlaceRandomNodes(int NumberOfNodes, float MinRadius, float MaxRadius, float MinDistance, float MaxDistance, FVector2D Origin) {
    for (int i = 0; i < NumberOfNodes; i++) {
        FVector2D location = FVector2D(FMath::RandRange(-10.0f, 10.0f), FMath::RandRange(-10.0f, 10.0f));
        float radius = FMath::RandRange(MinRadius, MaxRadius);
        float distance = FMath::RandRange(MinDistance, MaxDistance);

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
            if (node->SolveBorders() == true)
                resolvedAll = false;
        }
    }
}

void UAllocator::UpdateGraphs() {
    TArray<FAllocatorTriangle> triangles;

    // Create the super triangle that surrounds all nodes
    UAllocatorNode* superNodes[3];
    superNodes[0] = NewObject<UAllocatorNode>();
    superNodes[0]->Initialize(FVector2D(-SUPER_TRIANGLE_RANGE, -SUPER_TRIANGLE_RANGE), 0.0f, 0.0f);

    superNodes[1] = NewObject<UAllocatorNode>();
    superNodes[1]->Initialize(FVector2D(0, SUPER_TRIANGLE_RANGE), 0.0f, 0.0f);

    superNodes[2] = NewObject<UAllocatorNode>();
    superNodes[2]->Initialize(FVector2D(SUPER_TRIANGLE_RANGE, -SUPER_TRIANGLE_RANGE), 0.0f, 0.0f);
    FAllocatorTriangle superTriangle = FAllocatorTriangle(superNodes[0], superNodes[1], superNodes[2]);

    triangles.Add(superTriangle);

    for (UAllocatorNode* node : nodes) {
        // Find all triangles that the current node is inside
        TArray<FAllocatorTriangle> trianglesToRemove;
        for (FAllocatorTriangle triangle : triangles) {
            FVector2D circleOrigin;
            float circleRadius;
            triangle.GetCircumcircle(circleOrigin, circleRadius);

            if (FVector2D::Distance(circleOrigin, node->GetLocation()) <= circleRadius)
                trianglesToRemove.Add(triangle);
        }

        // Form a polygon from any triangles that contain the point
        TArray<FAllocatorEdge> polygon;
        for (FAllocatorTriangle badTriangle : trianglesToRemove) {
            for (FAllocatorEdge edge : badTriangle.edges) {
                // Only add unshared edges. This ensures that redundant triangles are removed
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

        // Create new triangles from each of the polygon edges
        for (FAllocatorEdge edge : polygon) {
            FAllocatorTriangle newTriangle = FAllocatorTriangle::CreateTriangleFromNodeAndEdge(node, edge);
            triangles.Add(newTriangle);
        }
    }

    // Find any edges with a super triangle and remove them from the graph
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

    // Create the Delauney graph using the edges from the triangulation
    for (FAllocatorTriangle triangle : triangles)
    for (FAllocatorEdge edge : triangle.edges) {
        delauneyGraph.edges.Add(edge);
    }

    // Create the minimum spanning tree from the newly generated delauney graph
    minimumSpanningTree = UAllocatorGraphFunctions::GetMinimumSpanningTree(delauneyGraph);

    // Queue the super nodes for destruction
    superNodes[0]->MarkAsGarbage();
    superNodes[1]->MarkAsGarbage();
    superNodes[2]->MarkAsGarbage();
}