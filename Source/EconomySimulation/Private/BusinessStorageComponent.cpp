// Fill out your copyright notice in the Description page of Project Settings.

#include "BusinessStorageComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UBusinessStorageComponent::UBusinessStorageComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBusinessStorageComponent::SaveStorage()
{

}

void UBusinessStorageComponent::LoadStorage()
{
}

// Called when the game starts
void UBusinessStorageComponent::BeginPlay()
{
	Super::BeginPlay();

	LoadStorage();
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

	FStorageItem *FoundItem = Storage.FindByPredicate([&](const FStorageItem &Item)
														{ return Item.ItemName == ItemName; });
	if (FoundItem)
	{
		FoundItem->Quantity += Quantity;
	}
	else
	{
		Storage.Add(FStorageItem(ItemName, Quantity));
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
