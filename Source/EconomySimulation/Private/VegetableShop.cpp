// Fill out your copyright notice in the Description page of Project Settings.

#include "VegetableShop.h"
#include "Kismet/GameplayStatics.h"
#include "GameManager.h"


AVegetableShop::AVegetableShop()
{
	PrimaryActorTick.bCanEverTick = true;
	ShopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShopMesh"));
	SetRootComponent(ShopMesh);
}

void AVegetableShop::SellItem(AMyPlayerCharacter *PlayerCharacter)
{
	if (!PlayerCharacter || !PlayerCharacter->PlayerInventoryComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter or PlayerInventory is null"));
		return;
	}
	UPlayerInventoryComponent *PlayerInventory = PlayerCharacter->PlayerInventoryComponent;
	TArray<FInventoryItem> &Inventory = PlayerInventory->Inventory;
	TMap<FName, int32> &ItemValues = PlayerInventory->ItemValues;

	int TotalValue = 0;

	for (const FInventoryItem &Item : Inventory)
	{
		int32* ItemValue = ItemValues.Find(Item.ItemName);
		if (ItemValue)
		{
			int32 TotalItemValue = Item.Quantity * (*ItemValue);
			TotalValue += TotalItemValue;

			UE_LOG(LogTemp, Warning, TEXT("Sold %d of %s for %d coins"), Item.Quantity, *Item.ItemName.ToString(), TotalItemValue);
		}
	}

	Inventory.Empty();
	ItemValues.Empty();

	GM->Profit += TotalValue;
	PlayerInventory->SaveInventory();
	GM->CalculateCoins();
	GM->SaveGame();
	UE_LOG(LogTemp, Warning, TEXT("Total coins earned from selling all items: %d"), TotalValue);
}

void AVegetableShop::BeginPlay()
{
	Super::BeginPlay();
	AActor *FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass());
	GM = Cast<AGameManager>(FoundActor);
}

void AVegetableShop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

