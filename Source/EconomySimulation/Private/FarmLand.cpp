// Fill out your copyright notice in the Description page of Project Settings.

#include "FarmLand.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "MyPlayerCharacter.h"
#include "GameManager.h"
#include "FarmSaveGame.h"

AFarmLand::AFarmLand()
{
    Carrots = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Carrots"));
    Carrots->SetupAttachment(LandMesh);
    Carrots->SetVisibility(true);

    StorageComponent = CreateDefaultSubobject<UBusinessStorageComponent>(TEXT("Storage"));

    FarmSetupCost = 300;
}
void AFarmLand::BeginPlay()
{
    Super::BeginPlay();
    RentLandForFarming();
    if (!IsInCinematic)
    {
        GetWorldTimerManager().SetTimer(ResourceGeneratingHandle, this, &AFarmLand::HarvestCropsToStorage, 120.0f, true);
    }
    APlayerController *PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    Player = Cast<AMyPlayerCharacter>(PlayerController->GetCharacter());
}

void AFarmLand::RentLandForFarming()
{
    bIsRented = true;
    GM->Expenses += FarmSetupCost;
    GM->CalculateCoins();
}

void AFarmLand::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AFarmLand::HarvestCropsToStorage()
{
    if (bIsRented && Player && Player->PlayerInventoryComponent)
    {
        // Define the probabilities
        const float WheatProbability = 0.6f;
        const float CarrotProbability = 0.3f;
        const float PotatoProbability = 0.1f;

        float RandomValue = FMath::FRand();

        FName AwardedCrop;
        int Value;
        if (RandomValue < WheatProbability)
        {
            AwardedCrop = "wheat";
            Value = 8;
        }
        else if (RandomValue < WheatProbability + CarrotProbability)
        {
            AwardedCrop = "carrot";
            Value = 10;
        }
        else
        {
            AwardedCrop = "potato";
            Value = 15;
        }
        int32 Quantity = FMath::RandRange(1, 3);
        StorageComponent->AddItem(AwardedCrop, Quantity, Value);
        AreItemsReady = true;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to harvest crops: Player or InventoryComponent not found"));
    }
}
void AFarmLand::TransferItems()
{
    if (Player && Player->PlayerInventoryComponent && StorageComponent)
    {
        for (const auto &Item : StorageComponent->GetItems())
        {
            Player->PlayerInventoryComponent->AddItem(Item.ItemName, Item.Quantity, Item.Value);
            StorageComponent->RemoveItem(Item.ItemName, Item.Quantity);
            UE_LOG(LogTemp, Warning, TEXT("TRANSFERRING Name: %s, Quantity: %d, Value : %d"), *Item.ItemName.ToString(), Item.Quantity, Item.Value);
            StorageComponent->SaveStorage();
            AreItemsReady = false;
        }
    }
}
void AFarmLand::HasItemsWithQuantityGreaterThanZero()
{
    for (const auto &Item : StorageComponent->GetItems())
    {
        if (Item.Quantity > 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("Item found with quantity > 0: %s, Quantity: %d"), *Item.ItemName.ToString(), Item.Quantity);
            AreItemsReady = true;
        }
    }
}
