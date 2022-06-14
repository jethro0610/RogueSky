// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AllocatorNode.generated.h"

USTRUCT(BlueprintType)
struct FAllocatorEdge {
	GENERATED_BODY()
	UAllocatorNode* nodes[2];

	void GetStandardForm(float &OutA, float &OutB, float &OutC) const;
	FVector2D GetMidpoint() const;
	void GetPerpindicularBisector(float& OutA, float& OutB, float& OutC) const;

	FAllocatorEdge() {

	}

	FAllocatorEdge(UAllocatorNode* node1, UAllocatorNode* node2) {
		nodes[0] = node1;
		nodes[1] = node2;
	}

	friend bool operator== (const FAllocatorEdge& A, const FAllocatorEdge& B) {
		if (A.nodes[0] == B.nodes[0] && A.nodes[1] == B.nodes[1])
			return true;

		if (A.nodes[1] == B.nodes[0] && A.nodes[0] == B.nodes[1])
			return true;

		return false;
	}

	friend bool operator!= (const FAllocatorEdge& A, const FAllocatorEdge& B) {
		if (A.nodes[0] == B.nodes[0] && A.nodes[1] == B.nodes[1])
			return false;

		if (A.nodes[1] == B.nodes[0] && A.nodes[0] == B.nodes[1])
			return false;

		return true;
	}
};

USTRUCT(BlueprintType)
struct FAllocatorTriangle {
	GENERATED_BODY()
	FAllocatorEdge edges[3];

	FAllocatorTriangle() {

	}

	FAllocatorTriangle(FAllocatorEdge edge1, FAllocatorEdge edge2, FAllocatorEdge edge3) {
		edges[0] = edge1;
		edges[1] = edge2;
		edges[2] = edge3;
	}

	FAllocatorTriangle(UAllocatorNode* node1, UAllocatorNode* node2, UAllocatorNode* node3) {
		edges[0].nodes[0] = node1;
		edges[0].nodes[1] = node2;

		edges[1].nodes[0] = node2;
		edges[1].nodes[1] = node3;

		edges[2].nodes[0] = node3;
		edges[2].nodes[1] = node1;
	}

	bool HasEdge(FAllocatorEdge Edge) const;
	void GetCircumcircle(FVector2D& Origin, float& Radius) const;
	static FAllocatorTriangle CreateTriangleFromNodeAndEdge(UAllocatorNode* Node, FAllocatorEdge Edge);

	friend bool operator== (const FAllocatorTriangle& A, const FAllocatorTriangle& B) {
		for (int i = 0; i < 3; i++) {
			bool hasEdge = false;
			for (int j = 0; j < 3; j++) {
				if (A.edges[i] == B.edges[j])
					hasEdge = true;
			}

			if (!hasEdge)
				return false;
		}

		return true;
	}

	friend uint32 GetTypeHash(const FAllocatorTriangle& Triangle) {
		uint32 Hash = FCrc::MemCrc32(&Triangle, sizeof(FAllocatorTriangle));
		return Hash;
	}
};

UCLASS()
class ROGUESKY_API UAllocatorNode : public UObject
{
	GENERATED_BODY()
	
private:
	FVector2D location;
	float radius;
	float minDistance;

public:
	UFUNCTION(BlueprintCallable)
		void Initialize(FVector2D Location, float Radius, float MinDistance) {
			location = Location;
			radius = Radius;
			minDistance = MinDistance;
		}

	UFUNCTION(BlueprintCallable)
		bool SolveCollision(UAllocatorNode* Other);

	UFUNCTION(BlueprintCallable)
		bool SolveBorders();

	UFUNCTION(BlueprintPure)
		FVector2D GetLocation() const { return location; }

	UFUNCTION(BlueprintPure)
		float GetRadius() const { return radius; }
};
