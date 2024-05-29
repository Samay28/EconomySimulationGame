// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Land.h"
#include "HouseLand.generated.h"

/**
 *
 */
UCLASS()
class ECONOMYSIMULATION_API AHouseLand : public ALand
{
	GENERATED_BODY()

public:
	AHouseLand();
	UPROPERTY(EditAnywhere)
	int PayCheck;

	UPROPERTY(EditAnywhere)
	int MaintenanceFees;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent *HouseMesh;

	void GetHouseRent();

	UFUNCTION(BlueprintCallable)
	void OwnHouse();

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	bool DoesOwnHouse;

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	int HouseConstructionCost;
};
