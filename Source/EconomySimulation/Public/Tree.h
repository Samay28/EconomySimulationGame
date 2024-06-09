// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tree.generated.h"

UCLASS()
class ECONOMYSIMULATION_API ATree : public AActor
{
	GENERATED_BODY()

public:
	ATree();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent *TreeMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent *LandForTreeMesh;

	int coinsToBeRewarded;
	int TimeToCut;
	int TimeToRegenerate;

	UFUNCTION(BlueprintCallable)
	void StartCuttingTrees();

	void RegenerateTime();
	void ProvideRewards();

	UPROPERTY(BlueprintReadWrite)
	bool CanBeCut;

	UPROPERTY(BlueprintReadWrite)
	bool IsCutting;

protected:
	virtual void BeginPlay() override;
	class AGameManager* GM;
	class AMyPlayerCharacter* Player;

private:
	FTimerHandle RewardTimerHandle;
	FTimerHandle RegenerationTimerHandle;

	FRotator InitialRotation;
};
