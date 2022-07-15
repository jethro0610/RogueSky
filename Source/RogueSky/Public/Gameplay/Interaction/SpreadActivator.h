// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Generation/LevelSections/LevelSection.h"
#include "SpreadActivator.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUESKY_API USpreadActivator : public UActorComponent {
	GENERATED_BODY()

public:	
	USpreadActivator();

private:
	void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	ALevelSection* currentSection = nullptr;

	UFUNCTION(BlueprintCallable)
		void SetCurrentSection(ALevelSection* Section) { currentSection = Section; }

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
