// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Generation/GenerationTypes.h"
#include "RuntimeMeshProvider.h"
#include "ChunkMeshProvider.generated.h"

class Chunk;
class OctreeChunkNode;

/**
 * 
 */
UCLASS()
class ROGUESKY_API UChunkMeshProvider : public URuntimeMeshProvider
{
	GENERATED_BODY()

private:
	mutable FCriticalSection chunkMutex;

	Chunk* chunk;
	UMaterialInterface* material;

	void GenerateVerts(TMap<FIntVector8, int>& IndexMap, int LODLevel, bool CalculateNormals, TFunctionRef<void (FVector Location, FVector Normal)> AddFunction);
	void GenerateTris(TMap<FIntVector8, int>& IndexMap, int LODLevel, TFunctionRef<void (int32 TriangleIndices[6])>);

protected:
	void Initialize() override;
	FBoxSphereBounds GetBounds() override;
	bool GetSectionMeshForLOD(int32 LODIndex, int32 SectionID, FRuntimeMeshRenderableMeshData& MeshData) override;
	

	FRuntimeMeshCollisionSettings GetCollisionSettings() override;
	bool GetCollisionMesh(FRuntimeMeshCollisionData& CollisionData) override;
	bool HasCollisionMesh() override;
	bool IsThreadSafe() override { return true; }
	
public:
	Chunk* GetChunk();
	void SetChunk(Chunk* Chunk);
	UMaterialInterface* GetChunkMaterial();
	void SetChunkMaterial(UMaterialInterface* Material);
	void UpdateMesh() { 
		MarkCollisionDirty();
		MarkSectionDirty(0, 0);
		//MarkSectionDirty(1, 0);
		//MarkSectionDirty(2, 0);
	};
};
