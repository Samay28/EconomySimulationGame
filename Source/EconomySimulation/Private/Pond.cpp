// Fill out your copyright notice in the Description page of Project Settings.

#include "Pond.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayerCharacter.h"
#include "PondSaveGame.h"
#include "Math/UnrealMathUtility.h"
#include "GameManager.h"

APond::APond()
{
    WaterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WaterMesh"));
    WaterMesh->SetupAttachment(LandMesh);

    PondSetupCost = 50;
    IsPurchased = false;
    count = 0;
    PondLevel = 1;

    PondID = FMath::Rand();
    LoadGame();
}
void APond::BeginPlay()
{
    Super::BeginPlay();
    if (count == 0)
    {
        SetupPond();
    }
    APlayerController *PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    Player = Cast<AMyPlayerCharacter>(PlayerController->GetCharacter());
    ProvideRewards();
    
}

void APond::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APond::SetupPond()
{
    if (GM->coins >= PondSetupCost && !IsPurchased)
    {
        PondLevel = 1;
        IsPurchased = true;
        GM->coins -= PondSetupCost;
        count++;
        SaveGame();
    }
}

void APond::ProvideRewards()
{
    int TroutAwardedTemp = FMath::RandRange(1, 4);
    int TunaAwardedTemp = FMath::RandRange(0, 2);
    int SalmonAwardedTemp = FMath::RandRange(0, 1);

    Player->PlayerInventoryComponent->AddItem("trout", TroutAwardedTemp, 15);
    Player->PlayerInventoryComponent->AddItem("tuna", TunaAwardedTemp, 25);
    Player->PlayerInventoryComponent->AddItem("salmon", SalmonAwardedTemp, 40);
    Player->PlayerInventoryComponent->SaveInventory();
    UE_LOG(LogTemp, Warning, TEXT("trout : %d, tuna : %d, salmon : %d"), TroutAwardedTemp, TunaAwardedTemp, SalmonAwardedTemp);
}

void APond::SaveGame()
{
    UPondSaveGame *SaveGameInstance;
    if (UGameplayStatics::DoesSaveGameExist(TEXT("PondSaveSlot"), 0))
    {
        SaveGameInstance = Cast<UPondSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("PondSaveSlot"), 0));
        if (!SaveGameInstance)
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to load save game instance from slot: PondSaveSlot"));
            return;
        }
    }
    else
    {
        SaveGameInstance = Cast<UPondSaveGame>(UGameplayStatics::CreateSaveGameObject(UPondSaveGame::StaticClass()));
        if (!SaveGameInstance)
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to create save game instance"));
            return;
        }
    }

    // Log to verify loading or creation of SaveGameInstance
    UE_LOG(LogTemp, Warning, TEXT("SaveGameInstance loaded or created"));

    FPondData PondData;
    PondData.PondID = PondID;
    PondData.IsPurchased = IsPurchased;
    PondData.count = count;
    UE_LOG(LogTemp, Warning, TEXT("Pond Saved1"));

    // Remove existing entry with the same FarmID
    bool bFoundExisting = false;
    for (int32 i = 0; i < SaveGameInstance->PondDataArray.Num(); ++i)
    {
        if (SaveGameInstance->PondDataArray[i].PondID == PondID)
        {
            SaveGameInstance->PondDataArray.RemoveAt(i);
            bFoundExisting = true;
            UE_LOG(LogTemp, Warning, TEXT("Pond Saved"));
            break;
        }
    }

    if (!bFoundExisting)
    {
        UE_LOG(LogTemp, Warning, TEXT("No existing entry with the same PondID found. Adding new entry."));
    }

    SaveGameInstance->PondDataArray.Add(PondData);

    if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("PondSaveSlot"), 0))
    {
        UE_LOG(LogTemp, Warning, TEXT("Pond data saved successfully to slot: Pond"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to save game to slot: PondSaveSlot"));
    }
}

void APond::LoadGame()
{
    UPondSaveGame *LoadGameInstance = Cast<UPondSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("PondSaveSlot"), 0));
    if (LoadGameInstance)
    {
        for (const FPondData &PondData : LoadGameInstance->PondDataArray)
        {
            if (PondData.PondID == PondID)
            {   
                UE_LOG(LogTemp, Error, TEXT("BAAAH"));
                IsPurchased = PondData.IsPurchased;
                count = PondData.count;
                break;
            }
        }
    }
}

