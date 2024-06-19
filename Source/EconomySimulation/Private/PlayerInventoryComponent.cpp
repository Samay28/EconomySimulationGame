// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerInventoryComponent.h"
#include "InventorySaveGame.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UPlayerInventoryComponent::UPlayerInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void UPlayerInventoryComponent::BeginPlay()
{
    Super::BeginPlay();

    LoadInventory();
}

void UPlayerInventoryComponent::SaveInventory()
{
    UInventorySaveGame *SaveGameInstance = Cast<UInventorySaveGame>(UGameplayStatics::CreateSaveGameObject(UInventorySaveGame::StaticClass()));

    for (const FInventoryItem &Item : Inventory)
    {
        SaveGameInstance->SavedInventory.Add(FInventoryItemData(Item.ItemName, Item.Quantity));
    }

    // Save the item values
    for (const auto &ItemValue : ItemValues)
    {
        SaveGameInstance->SavedItemValues.Add(ItemValue.Key, ItemValue.Value);
    }

    if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("PlayerInventorySlot"), 0))
    {
        UE_LOG(LogTemp, Warning, TEXT("Inventory saved successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to save inventory"));
    }
}

void UPlayerInventoryComponent::LoadInventory()
{
    UInventorySaveGame *LoadGameInstance = Cast<UInventorySaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerInventorySlot"), 0));
    if (LoadGameInstance)
    {
        Inventory.Empty();
        ItemValues.Empty(); // Ensure we clear the previous values before loading new ones

        for (const FInventoryItemData &ItemData : LoadGameInstance->SavedInventory)
        {
            Inventory.Add(FInventoryItem(ItemData.ItemName, ItemData.Quantity));
        }

        // Load the item values
        for (const auto &ItemValue : LoadGameInstance->SavedItemValues)
        {
            ItemValues.Add(ItemValue.Key, ItemValue.Value);
        }

        UE_LOG(LogTemp, Warning, TEXT("Inventory loaded successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load inventory"));
    }
}

// Called every frame
void UPlayerInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}

void UPlayerInventoryComponent::AddItem(FName ItemName, int32 Quantity, int32 Value)
{
    if (Quantity <= 0)
    {
        return;
    }

    FInventoryItem *FoundItem = Inventory.FindByPredicate([&](const FInventoryItem &Item) // If an item with the desired name is found, FindByPredicate returns a pointer to that item. If no matching item is found, it returns nullptr
                                                          { return Item.ItemName == ItemName; });

    if (FoundItem)
    {
        FoundItem->Quantity += Quantity;
    }
    else
    {
        Inventory.Add(FInventoryItem(ItemName, Quantity)); // Include Value
    }

    ItemValues.Add(ItemName, Value); // Set or update the value for the item
    SaveInventory();
}

void UPlayerInventoryComponent::RemoveItem(FName ItemName, int32 Quantity)
{
    if (Quantity <= 0)
    {
        return;
    }

    FInventoryItem *FoundItem = Inventory.FindByPredicate([&](const FInventoryItem &Item)
                                                          { return Item.ItemName == ItemName; });

    if (FoundItem)
    {
        FoundItem->Quantity -= Quantity;
        if (FoundItem->Quantity <= 0)
        {
            Inventory.RemoveSingle(*FoundItem);
            ItemValues.Remove(ItemName); // Remove the item value if the item is completely removed
        }

        SaveInventory();
    }
}

bool UPlayerInventoryComponent::HasItem(FName ItemName, int32 Quantity) const
{
    for (const FInventoryItem &Item : Inventory)
    {
        if (Item.ItemName == ItemName && Item.Quantity >= Quantity)
        {
            return true;
        }
    }
    return false;
}

int32 UPlayerInventoryComponent::GetItemQuantity(FName ItemName) const
{
    for (const FInventoryItem &Item : Inventory)
    {
        if (Item.ItemName == ItemName)
        {
            return Item.Quantity;
        }
    }
    return 0;
}
