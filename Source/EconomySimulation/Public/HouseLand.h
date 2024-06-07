// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Land.h"
#include "SaveGameInterface.h"
#include "HouseLand.generated.h"

/**
 *
 */
UCLASS()
class ECONOMYSIMULATION_API AHouseLand : public ALand, public ISaveGameInterface
{
	GENERATED_BODY()

public:
	AHouseLand();

	UPROPERTY(EditAnywhere)
	int PayCheck;

	UPROPERTY(EditAnywhere)
	int MaintenanceFees;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	bool DoesOwnHouse;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
    int32 HouseID;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent *HouseMesh;

	void GetHouseRent();

	UFUNCTION(BlueprintCallable)
	void OwnHouse();

	virtual void SaveGame() override;
	virtual void LoadGame() override;


protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	int HouseConstructionCost;
	int count;
};
