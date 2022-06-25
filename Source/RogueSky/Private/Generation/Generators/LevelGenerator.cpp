// Fill out your copyright notice in the Description page of Project Settings.
#include "Generation/Generators/LevelGenerator.h"
#include "Gameplay/LevelActors/Rail.h"
#include "Generation/ChunkManager.h"
#include "RogueSkyGameModeBase.h"

ALevelGenerator::ALevelGenerator() {
    allocator = CreateDefaultSubobject<UAllocator>("Allocator");
}

void ALevelGenerator::GenerateLevel() {
    allocator->PlaceRandomNodes(numberOfIslands, minRadius, maxRadius, minDistance, maxDistance, FVector2D::ZeroVector);
    allocator->AllocateNodes();
    allocator->UpdateGraphs();
    
    for (UAllocatorNode* node : allocator->GetNodes()) {
        IslandProperties properties;
        properties.origin = FVector(node->GetLocation().X, node->GetLocation().Y, 0.0f);
        properties.maxRadius = node->GetRadius();
        properties.minRadius = node->GetRadius() * 0.25f;
        UIslandGenerator* islandGenerator = NewObject<UIslandGenerator>();
        islandGenerator->Initialize(properties);
        islandGenerator->Generate();
        islandGenerators.Add(islandGenerator);
        nodeIslandMap.Add(node, islandGenerator);
    }

    AChunkManager* chunkManager = Cast<ARogueSkyGameModeBase>(GetWorld()->GetAuthGameMode())->GetChunkManager();
    for (UIslandGenerator* islandGenerator : islandGenerators) {
        for (auto pair : islandGenerator->distanceFields) {
            Chunk* chunk = chunkManager->GetChunk(pair.Key);
            if (chunk == nullptr) {
                UE_LOG(LogTemp, Warning, TEXT("Attempt to generate on invalid chunk at: %s"), *pair.Key.ToString());
                continue;
            }

            chunk->distanceField.AddDistanceField(pair.Value);
            chunk->SetState(ChunkState::HasDistanceField);
        }
    }

    GenerateConnections();
}

void ALevelGenerator::GenerateConnections() {
    UE_LOG(LogTemp, Warning, TEXT("Creating connections"));
    for (FAllocatorEdge& edge : allocator->GetMinimumSpanningTree().edges) {
        ARail* newRail = (ARail*)GetWorld()->SpawnActor(ARail::StaticClass());
        newRail->SetMesh(railMesh);
        newRail->ConnectIslands(nodeIslandMap[edge.nodes[0]], nodeIslandMap[edge.nodes[1]]);
    }
}
