// Fill out your copyright notice in the Description page of Project Settings.

#include "FarmLand.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "GameManager.h"
#include "FarmSaveGame.h"

AFarmLand::AFarmLand()
{
    Carrots = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Carrots"));
    Carrots->SetupAttachment(LandMesh);
    Carrots->SetVisibility(false);

    FarmSetupCost = 50;
    bCropsSowed = false;
    bIsRented = false;
    HarvestProfit = 20;
    CropsCost = 10;

    FarmID = FMath::Rand();
    LoadGame();
}
void AFarmLand::BeginPlay()
{
    Super::BeginPlay();
    RentLandForFarming();
    
}

void AFarmLand::RentLandForFarming()
{
    if (GM->coins >= FarmSetupCost && !bIsRented)
    {
        HarvestProfit = FMath::RandRange(5, 25);
        Carrots->SetVisibility(true);
        bCropsSowed = true;
        bIsRented = true;
        GM->coins -= FarmSetupCost;
        UE_LOG(LogTemp, Warning, TEXT("Farm Bought, Remaining money : %d"), GM->coins);
        GM->AddIncome(HarvestProfit);
        GM->AddExpenses(CropsCost);
        SaveGame();
    }
}
void AFarmLand::SaveGame()
{
    UFarmSaveGame* SaveGameInstance;
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
    FarmData.bCropsSowed = bCropsSowed;
    FarmData.HarvestProfit = HarvestProfit;
    FarmData.CropsCost = CropsCost;
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
    UE_LOG(LogTemp, Warning, TEXT("Farm Loaded nah"));
    UFarmSaveGame *LoadGameInstance = Cast<UFarmSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("FarmSaveSlot"), 0));
    if (LoadGameInstance)
    {   
        UE_LOG(LogTemp, Warning, TEXT("Farm Loaded nah2"));
        for (const FFarmData &FarmData : LoadGameInstance->FarmDataArray)
        {   
            UE_LOG(LogTemp, Warning, TEXT("Farm Loaded nah3"));
            if (FarmData.FarmID == FarmID)
            {   
                bIsRented = FarmData.bIsRented;
                bCropsSowed = FarmData.bCropsSowed;
                HarvestProfit = FarmData.HarvestProfit;
                CropsCost = FarmData.CropsCost;
                UE_LOG(LogTemp, Warning, TEXT("Farm Loaded"));
                Carrots->SetVisibility(bCropsSowed);
                break;
            }
        }
    }
}
void AFarmLand::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AFarmLand::HarvestCrops()
{
    if (bCropsSowed)
    {
        HarvestProfit = FMath::RandRange(5, 25);
        GM->coins -= CropsCost;
        GM->coins += HarvestProfit;
        UE_LOG(LogTemp, Warning, TEXT("Farm Profit : %d"), HarvestProfit);
    }
}
