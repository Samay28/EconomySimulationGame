// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGameManager.generated.h"

/**
 * 
 */
UCLASS()
class ECONOMYSIMULATION_API USaveGameManager : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	int32 Profit;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	int32 Expenses;
	
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	int IslandValue;
};
