// Fill out your copyright notice in the Description page of Project Settings.

#include "BusinessStorageComponent.h"
#include "StorageSaveClass.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UBusinessStorageComponent::UBusinessStorageComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBusinessStorageComponent::SaveStorage()
{
    UStorageSaveClass* SaveGameInstance = Cast<UStorageSaveClass>(UGameplayStatics::CreateSaveGameObject(UStorageSaveClass::StaticClass()));

    for (const FStorageItem& Item : Storage)
    {
        SaveGameInstance->SavedStorage.Add(FStorageItemData(Item.ItemName, Item.Quantity, Item.Value));
        UE_LOG(LogTemp, Warning, TEXT("Saving item - Name: %s, Quantity: %d, Value: %d"), *Item.ItemName.ToString(), Item.Quantity, Item.Value);
    }

    for (const auto& ItemValue : ItemValues)
    {
        SaveGameInstance->SavedItemValues.Add(ItemValue.Key, ItemValue.Value);
        UE_LOG(LogTemp, Warning, TEXT("Saving item value - Name: %s, Value: %d"), *ItemValue.Key.ToString(), ItemValue.Value);
    }

    if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("StorageSlot"), 0))
    {
        UE_LOG(LogTemp, Warning, TEXT("Storage saved successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to save storage"));
    }
}


void UBusinessStorageComponent::LoadStorage()
{
    UStorageSaveClass* LoadGameInstance = Cast<UStorageSaveClass>(UGameplayStatics::LoadGameFromSlot(TEXT("StorageSlot"), 0));
    if (LoadGameInstance)
    {
        Storage.Empty();
        ItemValues.Empty();

        UE_LOG(LogTemp, Warning, TEXT("Loaded game instance, number of saved items: %d"), LoadGameInstance->SavedStorage.Num());

        for (const FStorageItemData& ItemData : LoadGameInstance->SavedStorage)
        {
            Storage.Add(FStorageItem(ItemData.ItemName, ItemData.Quantity, ItemData.Value));
            UE_LOG(LogTemp, Warning, TEXT("Loaded item - Name: %s, Quantity: %d, Value: %d"), *ItemData.ItemName.ToString(), ItemData.Quantity, ItemData.Value);
        }

        UE_LOG(LogTemp, Warning, TEXT("Loaded game instance, number of saved item values: %d"), LoadGameInstance->SavedItemValues.Num());

        for (const auto& ItemValue : LoadGameInstance->SavedItemValues)
        {
            ItemValues.Add(ItemValue.Key, ItemValue.Value);
            UE_LOG(LogTemp, Warning, TEXT("Loaded item value - Name: %s, Value: %d"), *ItemValue.Key.ToString(), ItemValue.Value);
        }

        UE_LOG(LogTemp, Warning, TEXT("Storage loaded successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load storage"));
    }
}

// Called when the game starts
void UBusinessStorageComponent::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void UBusinessStorageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBusinessStorageComponent::AddItem(FName ItemName, int32 Quantity, int32 Value)
{
    if (Quantity <= 0)
        return;

    FStorageItem* FoundItem = Storage.FindByPredicate([&](const FStorageItem& Item)
    {
        return Item.ItemName == ItemName;
    });

    if (FoundItem)
    {
        FoundItem->Quantity += Quantity;
        FoundItem->Value = Value; // Ensure the value is updated if item already exists
    }
    else
    {
        Storage.Add(FStorageItem(ItemName, Quantity, Value));
    }
    ItemValues.Add(ItemName, Value);
    SaveStorage();
}

void UBusinessStorageComponent::RemoveItem(FName ItemName, int32 Quantity)
{
	if (Quantity <= 0)
	{
		return;
	}

	FStorageItem *FoundItem = Storage.FindByPredicate([&](const FStorageItem &Item)
													  { return Item.ItemName == ItemName; });

	if (FoundItem)
	{
		FoundItem->Quantity -= Quantity;
		if (FoundItem->Quantity <= 0)
		{
			Storage.RemoveSingle(*FoundItem);
			ItemValues.Remove(ItemName); // Remove the item value if the item is completely removed
		}

		SaveStorage();
	}
}

bool UBusinessStorageComponent::HasItem(FName ItemName, int32 Quantity) const
{
	for (const FStorageItem &Item : Storage)
	{
		if (Item.ItemName == ItemName && Item.Quantity >= Quantity)
		{
			return true;
		}
	}
	return false;
}

int32 UBusinessStorageComponent::GetItemQuantity(FName ItemName) const
{
	for (const FStorageItem &Item : Storage)
	{
		if (Item.ItemName == ItemName)
		{
			return Item.Quantity;
		}
	}
	return 0;
}

TArray<FStorageItem> UBusinessStorageComponent::GetItems() const
{
	return Storage;
}

void UBusinessStorageComponent::ClearItems()
{
	Storage.Empty();
}
