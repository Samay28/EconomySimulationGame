// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "StorageSaveClass.generated.h"

USTRUCT(BlueprintType)
struct FStorageItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Value;

	FStorageItemData()
		: ItemName(NAME_None), Quantity(0), Value(0)
	{
	}

	FStorageItemData(FName NewItemName, int32 NewQuantity, int32 NewValue)
		: ItemName(NewItemName), Quantity(NewQuantity), Value(NewValue)
	{
	}
};

UCLASS()
class ECONOMYSIMULATION_API UStorageSaveClass : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = Basic)
	TArray<FStorageItemData> SavedStorage;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	TMap<FName, int32> SavedItemValues;
};
