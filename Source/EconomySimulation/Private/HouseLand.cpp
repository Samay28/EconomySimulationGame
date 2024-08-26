#include "HouseLand.h"
#include "GameManager.h"
#include "HouseSaveGame.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

int AHouseLand::RentCollected = 0;

AHouseLand::AHouseLand()
{
    HouseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("House Mesh"));
    HouseMesh->SetupAttachment(LandMesh);
    HouseMesh->SetVisibility(true);

    HouseConstructionCost = 500;
    // DoesOwnHouse = false;
    PayCheck = 20;
    // count = 0;
}

void AHouseLand::BeginPlay()
{
    Super::BeginPlay();
    LoadGame();
    IsRentReady = false;

    OwnHouse();
    if (!IsInCinematic)
    {
        GetWorldTimerManager().SetTimer(HouseRent, this, &AHouseLand::GetHouseRent, 60.0f, true);
    }
}

void AHouseLand::GetHouseRent()
{
    IsRentReady = true;
    RentCollected += PayCheck;
    SaveGame();
}

void AHouseLand::OwnHouse()
{
    if (GM->coins >= HouseConstructionCost)
    {
        GM->CalculateCoins();
        GM->Expenses += HouseConstructionCost;
        GM->SaveGame();
        SaveGame();
        if (RentCollected > 0)
        {
            IsRentReady = true;
        }
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

    UE_LOG(LogTemp, Warning, TEXT("SaveGameInstance loaded or created"));

    FHouseData HouseData;
    HouseData.RentCollected = RentCollected;
    UE_LOG(LogTemp, Warning, TEXT("house : %d"), HouseData.RentCollected);

    bool bFoundExisting = false;
    for (int32 i = 0; i < SaveGameInstance->HouseDataArray.Num(); ++i)
    {
        SaveGameInstance->HouseDataArray[i] = HouseData;
        bFoundExisting = true;
        break;
    }

    if (!bFoundExisting)
    {
        SaveGameInstance->HouseDataArray.Add(HouseData);
    }

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
    if (LoadGameInstance && LoadGameInstance->HouseDataArray.Num() > 0)
    {
        for (const FHouseData &HouseData : LoadGameInstance->HouseDataArray)
        {
            RentCollected = HouseData.RentCollected;
            UE_LOG(LogTemp, Warning, TEXT("Loaded RentCollected: %d"), RentCollected);
            break;
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load save game from slot: HouseSaveSlot or no saved data found"));
    }
}

void AHouseLand::TransferRent()
{
    GM->Profit += RentCollected;
    GM->CalculateCoins();
    GM->SaveGame();
    RentCollected = 0;
    IsRentReady = false;
    UE_LOG(LogTemp, Warning, TEXT("HOUSE TRANSFERRED RENT"));
    SaveGame();
}
