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
    PayCheck = 10;
    RentCollected = 0;
    count = 0;

    // Generate a unique HouseID
}

void AHouseLand::BeginPlay()
{
    Super::BeginPlay();

    if (HouseID == 0)
    {
        HouseID = FMath::Rand();
    }
    LoadGame();
    SaveGame(); // Save the generated HouseID if it's new

    OwnHouse();
    GetWorldTimerManager().SetTimer(HouseRent, this, &AHouseLand::GetHouseRent, 10.0f, true);
}

void AHouseLand::GetHouseRent()
{
    if (DoesOwnHouse)
    {
        RentCollected += PayCheck;
        SaveGame();
        UE_LOG(LogTemp, Warning, TEXT("total Rent : %d"), RentCollected);
    }
}
void AHouseLand::OwnHouse()
{
    if (GM->coins >= HouseConstructionCost && !DoesOwnHouse)
    {
        DoesOwnHouse = true;
        HouseMesh->SetVisibility(true);
        bIsRented = true;
        GM->coins -= HouseConstructionCost;
        UE_LOG(LogTemp, Warning, TEXT("House Bought, Remaining money : %d"), GM->coins);
        GM->AddIncome(PayCheck);
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
    HouseData.RentCollected = RentCollected;
    HouseData.count = count;
    UE_LOG(LogTemp, Warning, TEXT("total Rent Saved As : %d"), HouseData.RentCollected);

    // Remove existing entry with the same id
    bool bFoundExisting = false;
    for (int32 i = 0; i < SaveGameInstance->HouseDataArray.Num(); ++i)
    {
        if (SaveGameInstance->HouseDataArray[i].HouseID == HouseID)
        {
            SaveGameInstance->HouseDataArray.RemoveAt(i);
            bFoundExisting = true;
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
    UHouseSaveGame *LoadGameInstance = Cast<UHouseSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("HouseSaveSlot"), 0));
    if (LoadGameInstance)
    {
        for (const FHouseData &HouseData : LoadGameInstance->HouseDataArray)
        {

            DoesOwnHouse = HouseData.DoesOwnHouse;
            PayCheck = HouseData.PayCheck;
            count = HouseData.count;
            RentCollected = HouseData.RentCollected;
            HouseMesh->SetVisibility(DoesOwnHouse);
            UE_LOG(LogTemp, Warning, TEXT("Loaded HouseID: %d, Rent: %d"), HouseID, RentCollected);
            return; 
        }

        // If no matching HouseID found, generate a new one and save it
        HouseID = FMath::Rand();
        SaveGame();
        UE_LOG(LogTemp, Warning, TEXT("Generated new HouseID: %d"), HouseID);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load save game from slot: HouseSaveSlot"));
        HouseID = FMath::Rand();
        SaveGame();
        UE_LOG(LogTemp, Warning, TEXT("Generated new HouseID: %d"), HouseID);
    }
}

void AHouseLand::TransferRent()
{
    GM->coins += RentCollected;
    RentCollected = 0;
    SaveGame();
    GM->SaveGame();
}
