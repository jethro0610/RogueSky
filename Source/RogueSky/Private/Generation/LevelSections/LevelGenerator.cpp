// Fill out your copyright notice in the Description page of Project Settings.
#include "Generation/LevelSections/LevelGenerator.h"
#include "Gameplay/Level/Rail.h"
#include "Generation/Chunk/ChunkManager.h"
#include "RogueSkyGameModeBase.h"

ALevelGenerator::ALevelGenerator() {
    
}

void ALevelGenerator::GenerateLevel() {
    allocator = NewObject<UAllocator>(this);

    FActorSpawnParameters sectionParameters;
    sectionParameters.bNoFail = true;
    for (TSubclassOf<ALevelSection> levelSection : levelSections) {
        ALevelSection* spawnedSection = GetWorld()->SpawnActor<ALevelSection>(levelSection, FVector::ZeroVector, FRotator::ZeroRotator, sectionParameters);
        UAllocatorNode* node = allocator->CreateNode(spawnedSection->GetRadius(), minDistance, maxDistance);
        nodeSectionMap.Add(node, spawnedSection);
    }
    allocator->AllocateNodes();
    allocator->UpdateGraphs();
    
    for (UAllocatorNode* node : allocator->GetNodes()) {
        ALevelSection* section = nodeSectionMap[node];
        section->SetActorLocation(FVector(node->GetLocation(), 0.0f));
        section->Generate();
    }

    AChunkManager* chunkManager = Cast<ARogueSkyGameModeBase>(GetWorld()->GetAuthGameMode())->GetChunkManager();
    for (TPair<UAllocatorNode*, ALevelSection*> nodeSectionPair : nodeSectionMap) {
        for (auto pair : nodeSectionPair.Value->GetDistanceFields()) {
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
        newRail->ConnectSections(nodeSectionMap[edge.nodes[0]], nodeSectionMap[edge.nodes[1]]);
    }
}
