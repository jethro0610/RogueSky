// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNode_SnakeTrail.h"
#include "AnimGraphNode_SkeletalControlBase.h"
#include "AnimGraphNode_SnakeTrail.generated.h"

UCLASS()
class ROGUESKY_API UAnimGraphNode_SnakeTrail : public UAnimGraphNode_SkeletalControlBase {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Settings)
		FAnimNode_SnakeTrail Node;

	virtual const FAnimNode_SkeletalControlBase* GetNode() const override { return &Node; }
	FText GetNodeTitle(ENodeTitleType::Type TitleType) const override { return FText::FromString("Snake Trail"); }
};
