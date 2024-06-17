// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "StorageSaveClass.generated.h"

USTRUCT(BlueprintType)
struct FStorageItemData
{
	GENERATED_BODY();

	FName ItemName;
	int32 Quantity;
	int32 Value;

	FStorageItemData() // This is the default constructor. It initializes an FStorageItem instance with default values.
		: ItemName(NAME_None), Quantity(0), Value(0)
	{
	}

	FStorageItemData(FName NewItemName, int32 NewQuantity, int32 NewValue) // This constructor allows the creation of an FStorageItem instance with specific values for ItemName and Quantity.
		: ItemName(NewItemName), Quantity(NewQuantity), Value(NewValue)
	{
	}
};

UCLASS()
class ECONOMYSIMULATION_API UStorageSaveClass : public USaveGame
{
	GENERATED_BODY()

public:
	TArray<FStorageItemData> SavedStorage;
	TMap<FName, int32> SavedItemValues;	
};
