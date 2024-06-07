// Fill out your copyright notice in the Description page of Project Settings.

#include "HouseLand.h"
#include "GameManager.h"
#include "HouseSaveGame.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"

AHouseLand::AHouseLand()
{
    HouseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("House Mesh"));
    HouseMesh->SetupAttachment(LandMesh);
    HouseMesh->SetVisibility(false);

    HouseConstructionCost = 50;
    bIsRented = false;
    DoesOwnHouse = false;
    PayCheck = 30;
    count = 0;
    MaintenanceFees = 10;

    HouseID = FMath::Rand();
    LoadGame();
}
void AHouseLand::BeginPlay()
{
    Super::BeginPlay();
    if (count == 0)
    {
        OwnHouse();
    }
}
void AHouseLand::GetHouseRent()
{
    if (DoesOwnHouse)
    {
        PayCheck = FMath::RandRange(5, 45);
        UE_LOG(LogTemp, Warning, TEXT("Credited House Rent"));
        GM->coins -= MaintenanceFees;
        GM->coins += PayCheck;
    }
}
void AHouseLand::OwnHouse()
{
    if (GM->coins >= HouseConstructionCost && !DoesOwnHouse)
    {
        PayCheck = FMath::RandRange(5, 45);
        DoesOwnHouse = true;
        HouseMesh->SetVisibility(true);
        bIsRented = true;
        GM->coins -= HouseConstructionCost;
        UE_LOG(LogTemp, Warning, TEXT("House Bought, Remaining money : %d"), GM->coins);
        GM->AddIncome(PayCheck);
        GM->AddExpenses(MaintenanceFees);
        count++;
        SaveGame();
    }
}
void AHouseLand::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
void AHouseLand::SaveGame()
{
    UHouseSaveGame *SaveGameInstance;
    if (UGameplayStatics::DoesSaveGameExist(TEXT("HouseSaveSlot"), 0))
    {
        SaveGameInstance = Cast<UHouseSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("HouseSaveSlot"), 0));
        if (!SaveGameInstance)
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to load save game instance from slot: HouseSaveSlot"));
            return;
        }
    }
    else
    {
        SaveGameInstance = Cast<UHouseSaveGame>(UGameplayStatics::CreateSaveGameObject(UHouseSaveGame::StaticClass()));
        if (!SaveGameInstance)
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to create save game instance for House"));
            return;
        }
    }

    // Log to verify loading or creation of SaveGameInstance
    UE_LOG(LogTemp, Warning, TEXT("SaveGameInstance loaded or created"));

    FHouseData HouseData;
    HouseData.HouseID = HouseID;
    HouseData.DoesOwnHouse = DoesOwnHouse;
    HouseData.PayCheck = PayCheck;
    HouseData.MaintenanceFees = MaintenanceFees;
    HouseData.count = count;
    UE_LOG(LogTemp, Warning, TEXT("House Saved1"));

    // Remove existing entry with the same FarmID
    bool bFoundExisting = false;
    for (int32 i = 0; i < SaveGameInstance->HouseDataArray.Num(); ++i)
    {
        if (SaveGameInstance->HouseDataArray[i].HouseID == HouseID)
        {
            SaveGameInstance->HouseDataArray.RemoveAt(i);
            bFoundExisting = true;
            UE_LOG(LogTemp, Warning, TEXT("House Saved"));
            break;
        }
    }

    if (!bFoundExisting)
    {
        UE_LOG(LogTemp, Warning, TEXT("No existing entry with the same HouseID found. Adding new entry."));
    }

    SaveGameInstance->HouseDataArray.Add(HouseData);

    if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("HouseSaveSlot"), 0))
    {
        UE_LOG(LogTemp, Warning, TEXT("House data saved successfully to slot: HouseSaveSlot"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to save game to slot: HouseSaveSlot"));
    }
}

void AHouseLand::LoadGame()
{
    UE_LOG(LogTemp, Warning, TEXT("House Loaded nah"));
    UHouseSaveGame *LoadGameInstance = Cast<UHouseSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("HouseSaveSlot"), 0));
    if (LoadGameInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("House Loaded nah2"));
        for (const FHouseData &HouseData : LoadGameInstance->HouseDataArray)
        {
            UE_LOG(LogTemp, Warning, TEXT("Farm Loaded nah3"));
            if (HouseData.HouseID == HouseID)
            {
                DoesOwnHouse = HouseData.DoesOwnHouse;
                PayCheck = HouseData.PayCheck;
                MaintenanceFees = HouseData.MaintenanceFees;
                count = HouseData.count;
                UE_LOG(LogTemp, Warning, TEXT("House Loaded"));
                HouseMesh->SetVisibility(DoesOwnHouse);
                break;
            }
        }
    }
}
