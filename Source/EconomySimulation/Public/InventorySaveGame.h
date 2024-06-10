#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "InventorySaveGame.generated.h"

USTRUCT(BlueprintType)
struct FInventoryItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity;

	FInventoryItemData()
		: ItemName(NAME_None), Quantity(0)
	{
	}

	FInventoryItemData(FName NewItemName, int32 NewQuantity)
		: ItemName(NewItemName), Quantity(NewQuantity)
	{
	}
};

UCLASS()
class ECONOMYSIMULATION_API UInventorySaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FInventoryItemData> SavedInventory;

	UPROPERTY()
	TMap<FName, int32> SavedItemValues;
};
