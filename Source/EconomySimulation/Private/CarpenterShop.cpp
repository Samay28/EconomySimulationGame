// Fill out your copyright notice in the Description page of Project Settings.
#include "CarpenterShop.h"
#include "Kismet/GameplayStatics.h"
#include "GameManager.h"

// Sets default values
ACarpenterShop::ACarpenterShop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    ShopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShopMesh"));
    SetRootComponent(ShopMesh);
}

// Called when the game starts or when spawned
void ACarpenterShop::BeginPlay()
{
	Super::BeginPlay();
	AActor *FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass());
	GM = Cast<AGameManager>(FoundActor);
}

// Called every frame
void ACarpenterShop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ACarpenterShop::SellItem(AMyPlayerCharacter* PlayerCharacter)
{
    if (!PlayerCharacter || !PlayerCharacter->PlayerInventoryComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter or PlayerInventory is null"));
        return;
    }

    UPlayerInventoryComponent* PlayerInventory = PlayerCharacter->PlayerInventoryComponent;
    TArray<FInventoryItem>& Inventory = PlayerInventory->Inventory;
    TMap<FName, int32>& ItemValues = PlayerInventory->ItemValues;

    int32 TotalValue = 0;

    for (const FInventoryItem& Item : Inventory)
    {
        int32* ItemValue = ItemValues.Find(Item.ItemName);
        if (ItemValue)
        {
            int32 TotalItemValue = Item.Quantity * (*ItemValue);
            TotalValue += TotalItemValue;

            UE_LOG(LogTemp, Warning, TEXT("Sold %d of %s for %d coins"), Item.Quantity, *Item.ItemName.ToString(), TotalItemValue);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No value found for item: %s"), *Item.ItemName.ToString());
        }
    }

    // Clear the inventory after selling everything
    Inventory.Empty();
    ItemValues.Empty();  // Clear item values as well

    GM->coins += TotalValue;
    PlayerInventory->SaveInventory();
    GM->SaveGame();
    UE_LOG(LogTemp, Warning, TEXT("Total coins earned from selling all items: %d"), TotalValue);
}
