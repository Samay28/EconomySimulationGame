// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Land.h"
#include "SaveGameInterface.h"
#include "FarmLand.generated.h"

/**
 *
 */
UCLASS()
class ECONOMYSIMULATION_API AFarmLand : public ALand, public ISaveGameInterface
{
	GENERATED_BODY()

public:
	AFarmLand();

	UPROPERTY(EditAnywhere)
	int CropsCost;

	UPROPERTY(EditAnywhere)
	int HarvestProfit;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCropsSowed;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent *Carrots;

	virtual void Tick(float DeltaTime) override;

	void HarvestCrops();

	UFUNCTION(BlueprintCallable)
	void RentLandForFarming();

	virtual void SaveGame() override;

	virtual void LoadGame() override;

protected:
	virtual void BeginPlay() override;
	int FarmSetupCost;

private:

	UPROPERTY(EditAnywhere, Category = "Farm")
    int32 FarmID;
};
