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

void UPlayerInventoryComponent::AddItem(FName ItemName, int32 Quantity)
{
	for(FInventoryItem& Item : Inventory)
	{
		if(Item.ItemName == ItemName)
		{
			Item.Quantity+=Quantity;
			return;
		}
	}
	Inventory.Add(FInventoryItem(ItemName,Quantity));
}

void UPlayerInventoryComponent::RemoveItem(FName ItemName, int32 Quantity)
{
    // Find the item in the inventory
    for (int32 i = 0; i < Inventory.Num(); ++i)
    {
        if (Inventory[i].ItemName == ItemName)
        {
            // Item found, decrease the quantity
            Inventory[i].Quantity -= Quantity;

            // Remove the item if the quantity is zero or less
            if (Inventory[i].Quantity <= 0)
            {
                Inventory.RemoveAt(i);
            }
            return;
        }
    }
}

// bool UPlayerInventoryComponent::HasItem(FName ItemName, int32 Quantity) const
// {
//     for (const FInventoryItem& Item : InventoryItems)
//     {
//         if (Item.ItemName == ItemName && Item.Quantity >= Quantity)
//         {
//             return true;
//         }
//     }
//     return false;
// }
