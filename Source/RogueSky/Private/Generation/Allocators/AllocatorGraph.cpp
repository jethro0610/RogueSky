// Fill out your copyright notice in the Description page of Project Settings.
#include "Generation/Allocators/AllocatorGraph.h"

void UAllocatorGraphFunctions::GetEdgesConnectedToNode(const FAllocatorGraph& Graph, UAllocatorNode* Node, TArray<FAllocatorEdge>& OutEdges) {
    for (FAllocatorEdge edge : Graph.edges) {
        for (UAllocatorNode* otherNode : edge.nodes) {
            if (otherNode == Node)
                OutEdges.Add(edge);
        }
    }
}

void UAllocatorGraphFunctions::GetNodesInGraph(const FAllocatorGraph& Graph, TSet<UAllocatorNode*>& Nodes) {
    for (FAllocatorEdge edge : Graph.edges) {
        for (UAllocatorNode* node : edge.nodes) {
            Nodes.Add(node);
        }
    }
}

float UAllocatorGraphFunctions::GetEdgeLength(const FAllocatorEdge& Edge) {
    return FVector2D::Distance(Edge.nodes[0]->GetLocation(), Edge.nodes[1]->GetLocation());
}

FAllocatorGraph UAllocatorGraphFunctions::GetMinimumSpanningTree(const FAllocatorGraph& Graph) {
    // Get the nodes in the graph and create a list of visited nodes
    FAllocatorGraph minimumSpanningTree;
    TSet<UAllocatorNode*> nodesInGraph;
    GetNodesInGraph(Graph, nodesInGraph);
    TMap<UAllocatorNode*, bool> visitedNodes;
    
    // Mark all nodes as unvisited except the first one
    bool isFirstValue = true;
    for (UAllocatorNode* node : nodesInGraph) {
        visitedNodes.Add(node, isFirstValue);
        isFirstValue = false;
    }

    // Generate n - 1 edges
    for (int i = 0; i < nodesInGraph.Num() - 1; i++) {
        // Get any edges connected to nodes that have been visisted
        TArray<FAllocatorEdge> connectedEdges;
        for (TPair<UAllocatorNode*, bool> nodePair : visitedNodes) {
            if (nodePair.Value == false) // The node wasn't visisted so skip to whatever's next in the list
                continue;

            GetEdgesConnectedToNode(Graph, nodePair.Key, connectedEdges);
        }

        // Get the shortest edge in the list of edges that are connected to a visisted node
        FAllocatorEdge shortestEdge;
        float shortestLength = INFINITY;
        UAllocatorNode* nodeToFlag = nullptr;
        for (FAllocatorEdge edge : connectedEdges) {
            float edgeLength = GetEdgeLength(edge);
            if (edgeLength > shortestLength) // Skip the edge if it isn't shorter than an already checked edge
                continue;

            // Mark the new shortest edge
            for (UAllocatorNode* node : edge.nodes) {
                if (visitedNodes[node] == false) {
                    shortestLength = edgeLength;
                    nodeToFlag = node;
                    shortestEdge = edge;
                }
            }
        }

        // Add the edge to the graph and mark the node as visited
        minimumSpanningTree.edges.Add(shortestEdge);
        visitedNodes[nodeToFlag] = true;
    }

    return minimumSpanningTree;
}