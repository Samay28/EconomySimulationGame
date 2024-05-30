// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "HouseSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FHouseData
{	
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	int PayCheck;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	int MaintenanceFees;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	bool DoesOwnHouse;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	int32 HouseID;
};

UCLASS()
class ECONOMYSIMULATION_API UHouseSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	TArray<FHouseData> HouseDataArray;
};
