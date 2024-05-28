// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Land.h"
#include "FarmLand.generated.h"

/**
 *
 */
UCLASS()
class ECONOMYSIMULATION_API AFarmLand : public ALand
{
	GENERATED_BODY()

public:
	AFarmLand();

	UPROPERTY(EditAnywhere)
	int CropsCost;

	UPROPERTY(EditAnywhere)
	int HarvestProfit;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	bool bCropsSowed;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Carrots;
	
	virtual void Tick(float DeltaTime) override;

	void HarvestCrops();

	UFUNCTION(BlueprintCallable)
	void RentLandForFarming();
	

protected:
	virtual void BeginPlay() override;

	int FarmSetupCost;
};
