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
    Carrots->SetVisibility(false);

    StorageComponent = CreateDefaultSubobject<UBusinessStorageComponent>(TEXT("Storage"));

    FarmSetupCost = 50;
    bIsRented = false;
    count = 0;

    FarmID = FMath::Rand();
    LoadGame();
}
void AFarmLand::BeginPlay()
{
    Super::BeginPlay();
    if (count == 0)
    {
        RentLandForFarming();
    }
   if(bIsRented)
   {
        GetWorldTimerManager().SetTimer(ResourceGeneratingHandle, this, &AFarmLand::HarvestCropsToStorage, 10.0f, true);
    }
    APlayerController *PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    Player = Cast<AMyPlayerCharacter>(PlayerController->GetCharacter());
}

void AFarmLand::RentLandForFarming()
{
    if (GM->coins >= FarmSetupCost && !bIsRented)
    {
        // HarvestProfit = FMath::RandRange(5, 25);
        Carrots->SetVisibility(true);
        bIsRented = true;
        GM->coins -= FarmSetupCost;
        UE_LOG(LogTemp, Warning, TEXT("Farm Bought, Remaining money : %d"), GM->coins);
        count++;
        SaveGame();
    }
}
void AFarmLand::SaveGame()
{
    UFarmSaveGame *SaveGameInstance;
    if (UGameplayStatics::DoesSaveGameExist(TEXT("FarmSaveSlot"), 0))
    {
        SaveGameInstance = Cast<UFarmSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("FarmSaveSlot"), 0));
        if (!SaveGameInstance)
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to load save game instance from slot: FarmSaveSlot"));
            return;
        }
    }
    else
    {
        SaveGameInstance = Cast<UFarmSaveGame>(UGameplayStatics::CreateSaveGameObject(UFarmSaveGame::StaticClass()));
        if (!SaveGameInstance)
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to create save game instance"));
            return;
        }
    }

    // Log to verify loading or creation of SaveGameInstance
    UE_LOG(LogTemp, Warning, TEXT("SaveGameInstance loaded or created"));

    FFarmData FarmData;
    FarmData.FarmID = FarmID;
    FarmData.bIsRented = bIsRented;
    FarmData.count = count;
    UE_LOG(LogTemp, Warning, TEXT("Farm Saved1"));

    // Remove existing entry with the same FarmID
    bool bFoundExisting = false;
    for (int32 i = 0; i < SaveGameInstance->FarmDataArray.Num(); ++i)
    {
        if (SaveGameInstance->FarmDataArray[i].FarmID == FarmID)
        {
            SaveGameInstance->FarmDataArray.RemoveAt(i);
            bFoundExisting = true;
            UE_LOG(LogTemp, Warning, TEXT("Farm Saved"));
            break;
        }
    }

    if (!bFoundExisting)
    {
        UE_LOG(LogTemp, Warning, TEXT("No existing entry with the same FarmID found. Adding new entry."));
    }

    SaveGameInstance->FarmDataArray.Add(FarmData);

    if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("FarmSaveSlot"), 0))
    {
        UE_LOG(LogTemp, Warning, TEXT("Farm data saved successfully to slot: FarmSaveSlot"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to save game to slot: FarmSaveSlot"));
    }
}

void AFarmLand::LoadGame()
{
    UFarmSaveGame *LoadGameInstance = Cast<UFarmSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("FarmSaveSlot"), 0));
    if (LoadGameInstance)
    {
        for (const FFarmData &FarmData : LoadGameInstance->FarmDataArray)
        {
            if (FarmData.FarmID == FarmID)
            {
                bIsRented = FarmData.bIsRented;
                count = FarmData.count;
                Carrots->SetVisibility(bIsRented);
                break;
            }
        }
    }
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

        // Generate a random float between 0 and 1
        float RandomValue = FMath::FRand();

        FName AwardedCrop;
        int Value;
        if (RandomValue < WheatProbability)
        {
            AwardedCrop = "wheat";
            Value = 10;
        }
        else if (RandomValue < WheatProbability + CarrotProbability)
        {
            AwardedCrop = "carrot";
            Value = 20;
        }
        else
        {
            AwardedCrop = "potato";
            Value = 25;
        }
        int32 Quantity = FMath::RandRange(1, 3);
        Player->PlayerInventoryComponent->AddItem(AwardedCrop, Quantity, Value);

        // Log the awarded crop and quantity for debugging
        UE_LOG(LogTemp, Warning, TEXT("Harvested: %s, Quantity: %d"), *AwardedCrop.ToString(), Quantity);
        Player->PlayerInventoryComponent->SaveInventory();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to harvest crops: Player or InventoryComponent not found"));
    }
}
