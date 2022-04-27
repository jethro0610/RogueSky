// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNode_SnakeTrail.h"
#include "Animation/AnimInstanceProxy.h"
#include "Kismet/KismetMathLibrary.h"
#include "AnimationCoreLibrary.h"
#include "Engine.h"

void FAnimNode_SnakeTrail::EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms) {
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(EvaluateSkeletalControl_AnyThread);
	const FBoneContainer& boneContainer = Output.Pose.GetPose().GetBoneContainer();
	FAxis lookAxis = FAxis(FVector(0, -1.0f, 0.0f));
	const float deltaTime = Output.AnimInstanceProxy->GetDeltaSeconds();

	// Set the location of the first bone
	const FTransform componentToWorld = Output.AnimInstanceProxy->GetComponentTransform();
	const FVector currentLocation = Output.AnimInstanceProxy->GetComponentTransform().GetLocation();
	boneLocations[0] = Output.Pose.GetComponentSpaceTransform(referenceBones[0].GetCompactPoseIndex(boneContainer)).GetRelativeTransform(componentToWorld.Inverse()).GetLocation();

	FVector deltaLocation = FVector::ZeroVector;
	if (previousLocation != FVector::ZeroVector)
		deltaLocation = (currentLocation - previousLocation) * movementTrackingSpeed;

	// Loop through all bones
	for (int i = 1; i < bonesToModify.Num(); i++) {
		// Set any zero vectors to the bones reference location
		const FVector referenceLocation = Output.Pose.GetComponentSpaceTransform(referenceBones[i].GetCompactPoseIndex(boneContainer)).GetRelativeTransform(componentToWorld.Inverse()).GetLocation();
		const FVector previousReferenceLocation = Output.Pose.GetComponentSpaceTransform(referenceBones[i - 1].GetCompactPoseIndex(boneContainer)).GetRelativeTransform(componentToWorld.Inverse()).GetLocation();
		if (boneLocations[i] == FVector::ZeroVector)
			boneLocations[i] = referenceLocation;

		if (deltaLocation != FVector::ZeroVector)
			boneLocations[i] += deltaLocation;

		// Get the location of previous bone and its offset
		FVector previousBoneLocation = boneLocations[i - 1];
		FVector referenceOffset = referenceLocation - previousReferenceLocation;

		// Ensure the bones distance isn't higher than the reference bone
		float distance = referenceOffset.Size();
		FVector boneUnitVector = (boneLocations[i] - previousBoneLocation).GetSafeNormal();
		boneLocations[i] = previousBoneLocation + boneUnitVector * distance;

		// Lerp the velocity
		FVector desiredVelocity = (previousBoneLocation + referenceOffset) - boneLocations[i];
		boneVelocities[i] = UKismetMathLibrary::VInterpTo(boneVelocities[i], desiredVelocity, deltaTime, velocitySpeed);

		// Lerp the position
		FVector desiredPosition = boneLocations[i] + boneVelocities[i];
		boneLocations[i] = UKismetMathLibrary::VInterpTo(boneLocations[i], desiredPosition, deltaTime, locationSpeed);
		boneUnitVector = (boneLocations[i] - previousBoneLocation).GetSafeNormal();
		boneLocations[i] = previousBoneLocation + boneUnitVector * distance;

		// Get the location of the bone in component space
		FTransform TargetTransform = FBoneSocketTarget().GetTargetTransform(boneLocations[i], Output.Pose, componentToWorld);
		FVector TargetLocationInComponentSpace = TargetTransform.GetLocation();

		// Make the previous bone look at the current bone
		const FCompactPoseBoneIndex ModifyBoneIndex = bonesToModify[i - 1].GetCompactPoseIndex(boneContainer);
		FTransform ComponentBoneTransform = Output.Pose.GetComponentSpaceTransform(ModifyBoneIndex);
		FVector LookAtVector = lookAxis.GetTransformedAxis(ComponentBoneTransform);
		FQuat DeltaRotation = AnimationCore::SolveAim(ComponentBoneTransform, TargetLocationInComponentSpace, LookAtVector);
		ComponentBoneTransform.SetRotation(DeltaRotation * ComponentBoneTransform.GetRotation());

		// Update the bone
		Output.Pose.SetComponentSpaceTransform(ModifyBoneIndex, ComponentBoneTransform);
		OutBoneTransforms.Add(FBoneTransform(ModifyBoneIndex, ComponentBoneTransform));
	}
	previousLocation = currentLocation;
}

void FAnimNode_SnakeTrail::InitializeBoneReferences(const FBoneContainer& RequiredBones) {
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(InitializeBoneReferences)

	GatherBoneReferences(RequiredBones.GetReferenceSkeleton(), startModifyBone, endModifyBone, bonesToModify);
	GatherBoneReferences(RequiredBones.GetReferenceSkeleton(), startReferenceBone, endReferenceBone, referenceBones);

	boneLocations.Init(FVector::ZeroVector, bonesToModify.Num());
	boneVelocities.Init(FVector::ZeroVector, bonesToModify.Num());

	if (referenceBones.Num() != bonesToModify.Num())
		return;

	for (int i = 0; i < bonesToModify.Num(); i++) {
		bonesToModify[i].Initialize(RequiredBones);
		referenceBones[i].Initialize(RequiredBones);
	}
}

void FAnimNode_SnakeTrail::GatherBoneReferences(const FReferenceSkeleton& RefSkeleton, const FBoneReference StartBone, const FBoneReference EndBone, TArray<FBoneReference>& Bones) {
	Bones.Reset();

	int32 startIndex = RefSkeleton.FindBoneIndex(StartBone.BoneName);
	int32 endIndex = RefSkeleton.FindBoneIndex(EndBone.BoneName);

	if (startIndex != INDEX_NONE && endIndex != INDEX_NONE) {
		// Loop through the bones from end to start
		int32 boneIndex = endIndex;
		while (boneIndex != startIndex) {
			if (boneIndex == INDEX_NONE) {
				Bones.Reset();
				break;
			}

			FName boneName = RefSkeleton.GetBoneName(boneIndex);
			Bones.EmplaceAt(0, boneName);
			boneIndex = RefSkeleton.GetParentIndex(boneIndex);
		}

		// Add the start bone
		if (Bones.Num()) {
			FName boneName = RefSkeleton.GetBoneName(startIndex);
			Bones.EmplaceAt(0, boneName);
		}
	}
}

bool FAnimNode_SnakeTrail::IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones) {
	return bonesToModify.Num() > 0 && referenceBones.Num() == bonesToModify.Num();
}