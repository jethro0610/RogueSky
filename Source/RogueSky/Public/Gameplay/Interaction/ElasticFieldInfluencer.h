// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RogueSkyGameModeBase.h"
#include "ElasticFieldInfluencer.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUESKY_API UElasticFieldInfluencer : public UActorComponent {
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UElasticFieldInfluencer();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere)
		UMaterial* influenceMaterial;

	UPROPERTY(EditAnywhere)
		float influenceRadius;

	UPROPERTY()
		UMaterialInstanceDynamic* dynamicInfluenceMaterial;
	ARogueSkyGameModeBase* gamemode;

public:
	UFUNCTION(BlueprintCallable)
		void SetRadius(float Radius);
};
