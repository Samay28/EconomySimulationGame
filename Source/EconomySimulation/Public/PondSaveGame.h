// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PondSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FPondData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	bool IsPurchased;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	int32 PondID;
	
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	int count;
};
UCLASS()
class ECONOMYSIMULATION_API UPondSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	TArray<FPondData> PondDataArray;
	
};
