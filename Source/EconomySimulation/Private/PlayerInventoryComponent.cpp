// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInventoryComponent.h"

// Sets default values for this component's properties
UPlayerInventoryComponent::UPlayerInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPlayerInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
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

    FInventoryItem* FoundItem = Inventory.FindByPredicate([&](const FInventoryItem& Item) { return Item.ItemName == ItemName; });

    if (FoundItem)
    {
        FoundItem->Quantity += Quantity;
    }
    else
    {
        Inventory.Add(FInventoryItem(ItemName, Quantity));
        ItemValues.Add(ItemName, Value);  // Set the value for the new item
    }
}


void UPlayerInventoryComponent::RemoveItem(FName ItemName, int32 Quantity)
{
    if (Quantity <= 0)
    {
        return;
    }

    FInventoryItem* FoundItem = Inventory.FindByPredicate([&](const FInventoryItem& Item) { return Item.ItemName == ItemName; });

    if (FoundItem)
    {
        FoundItem->Quantity -= Quantity;
        if (FoundItem->Quantity <= 0)
        {
            Inventory.RemoveSingle(*FoundItem);
            ItemValues.Remove(ItemName);  // Remove the item value if the item is completely removed
        }
    }
}


bool UPlayerInventoryComponent::HasItem(FName ItemName, int32 Quantity) const
{
    for (const FInventoryItem& Item : Inventory)
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
    for (const FInventoryItem& Item : Inventory)
	{
		if (Item.ItemName == ItemName)
		{
			return Item.Quantity;
		}
	}
	return 0;
}
