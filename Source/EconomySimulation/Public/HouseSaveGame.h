// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "HouseSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FHouseData
{	
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int PayCheck;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 RentCollected;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool DoesOwnHouse;

	// UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	// int32 HouseID;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 count;
};

UCLASS()
class ECONOMYSIMULATION_API UHouseSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	TArray<FHouseData> HouseDataArray;
};
