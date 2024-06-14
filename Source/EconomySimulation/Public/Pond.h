// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Land.h"
#include "Pond.generated.h"

/**
 *
 */
UCLASS()
class ECONOMYSIMULATION_API APond : public ALand
{
	GENERATED_BODY()

public:
	APond();

	UPROPERTY(EditAnywhere)
	int CleaningCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsPurchased;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent *WaterMesh;

	virtual void Tick(float DeltaTime) override;

	void ProvideRewards();

	UFUNCTION(BlueprintCallable)
	void SetupPond();

	void SaveGame();
	void LoadGame();

	int PondLevel;
	// class MyPlayerCharacter* Player;

protected:
	virtual void BeginPlay() override;
	int PondSetupCost;
	int count;

private:
	UPROPERTY(EditAnywhere, Category = "Pond")
	int32 PondID;
};
