// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BoneControllers/AnimNode_SkeletalControlBase.h"
#include "CommonAnimTypes.h"
#include "AnimNode_SnakeTrail.generated.h"

USTRUCT()
struct ROGUESKY_API FAnimNode_SnakeTrail : public FAnimNode_SkeletalControlBase {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Bones")
		FBoneReference startReferenceBone;
	UPROPERTY(EditAnywhere, Category = "Bones")
		FBoneReference endReferenceBone;
	UPROPERTY(EditAnywhere, Category = "Bones")
		FBoneReference startModifyBone;
	UPROPERTY(EditAnywhere, Category = "Bones")
		FBoneReference endModifyBone;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (PinHiddenByDefault))
		float movementTrackingSpeed = 0.8f;
	UPROPERTY(EditAnywhere, Category = "Speed", meta = (PinHiddenByDefault))
		float locationSpeed = 8.0f;
	UPROPERTY(EditAnywhere, Category = "Speed", meta = (PinHiddenByDefault))
		float velocitySpeed = 12.0f;

	virtual void EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms) override;
	virtual bool IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones) override;
	void GatherBoneReferences(const FReferenceSkeleton& RefSkeleton, const FBoneReference StartBone, const FBoneReference EndBone, TArray<FBoneReference>& Bones);

private:
	TArray<FVector> boneLocations;
	TArray<FVector> boneVelocities;
	TArray<FBoneReference> referenceBones;
	TArray<FBoneReference> bonesToModify;
	FVector previousLocation = FVector::ZeroVector;

	virtual void InitializeBoneReferences(const FBoneContainer& RequiredBones) override;
};
