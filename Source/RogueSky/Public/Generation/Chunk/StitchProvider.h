// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RuntimeMeshProvider.h"
#include "StitchProvider.generated.h"

class Chunk;

/**
 * 
 */
UCLASS()
class ROGUESKY_API UStitchProvider : public URuntimeMeshProvider
{
	GENERATED_BODY()
private:
	mutable FCriticalSection chunkMutex;

	Chunk* chunk;
	UMaterialInterface* material;

	void GenerateStitchVertsAndTris(uint8 LODLevel, bool CalculateNormals, int& VertCount, TFunctionRef<void (FVector Location, FVector Normal)> VertexFunction, TFunctionRef<void (int32 TriangleIndicies[3])> TriFunction);
	void GenerateCornerVertsAndTris(uint8 LODLevel, bool CalculateNormals, int& VertCount, TFunctionRef<void (FVector Location, FVector Normal)> VertexFunction, TFunctionRef<void (int32 TriangleIndicies[3])> TriFunction);

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
	UMaterialInterface* GetStitchMaterial();
	void SetStitchMaterial(UMaterialInterface* Material);
	void UpdateStitch() { 
		MarkCollisionDirty();
		MarkSectionDirty(0, 0);
		//MarkSectionDirty(1, 0);
		//MarkSectionDirty(2, 0);
	};
};
