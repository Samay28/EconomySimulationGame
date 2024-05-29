// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "FarmSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FFarmData
{
    GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
    int32 FarmID;
	
    UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
    bool bIsRented;

    UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
    bool bCropsSowed;

    UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
    float HarvestProfit;
    
    UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
    float CropsCost;
};

UCLASS()
class ECONOMYSIMULATION_API UFarmSaveGame : public USaveGame
{
	GENERATED_BODY()

public:	
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
    TArray<FFarmData> FarmDataArray;
	
};
