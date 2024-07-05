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
class ECONOMYSIMULATION_API AHouseLand : public ALand
{
	GENERATED_BODY()

public:
	AHouseLand();

	UPROPERTY(EditAnywhere)
	int PayCheck;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool DoesOwnHouse;

	// UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	// int32 HouseID;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent *HouseMesh;

	void GetHouseRent();

	UFUNCTION(BlueprintCallable)
	void OwnHouse();

	void SaveGame();
	void LoadGame();
	

	UFUNCTION(BlueprintCallable)
	void TransferRent();

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	int HouseConstructionCost;
	// int count;

	FTimerHandle HouseRent;

	static int RentCollected;
};
