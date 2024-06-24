// Fill out your copyright notice in the Description page of Project Settings.

#include "FishShop.h"
#include "Kismet/GameplayStatics.h"
#include "GameManager.h"

// Sets default values
AFishShop::AFishShop()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ShopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShopMesh"));
	SetRootComponent(ShopMesh);
}

// Called when the game starts or when spawned
void AFishShop::BeginPlay()
{
	Super::BeginPlay();
	AActor *FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass());
	GM = Cast<AGameManager>(FoundActor);
}

// Called every frame
void AFishShop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFishShop::SellItem(AMyPlayerCharacter *PlayerCharacter)
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
		int32 *ItemValue = ItemValues.Find(Item.ItemName);
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
	UE_LOG(LogTemp, Warning, TEXT("Total coins earned from selling all items: %d"), TotalValue);
}
