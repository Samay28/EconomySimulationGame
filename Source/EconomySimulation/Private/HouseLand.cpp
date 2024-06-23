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

    HouseConstructionCost = 50;
    DoesOwnHouse = false;
    PayCheck = 10;
    count = 0;
}

void AHouseLand::BeginPlay()
{
    Super::BeginPlay();
    GM->LoadGame();
    LoadGame();

    if (!DoesOwnHouse)
    {
        OwnHouse();
    }

    GetWorldTimerManager().SetTimer(HouseRent, this, &AHouseLand::GetHouseRent, 10.0f, true);
}

void AHouseLand::GetHouseRent()
{
    if (DoesOwnHouse)
    {
        RentCollected += PayCheck;
        SaveGame();
    }
}

void AHouseLand::OwnHouse()
{
    if (GM->coins >= HouseConstructionCost && !DoesOwnHouse)
    {
        DoesOwnHouse = true;
        HouseMesh->SetVisibility(true);
        GM->coins -= HouseConstructionCost;
        GM->SaveGame(); // Save the game manager state after deducting coins
        UE_LOG(LogTemp, Warning, TEXT("House Bought, Remaining money : %d"), GM->coins);
        HouseID = FMath::Rand();
        UE_LOG(LogTemp, Warning, TEXT("House id : %d"), HouseID);
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

    UE_LOG(LogTemp, Warning, TEXT("SaveGameInstance loaded or created"));

    FHouseData HouseData;
    HouseData.HouseID = HouseID;
    HouseData.DoesOwnHouse = DoesOwnHouse;
    HouseData.RentCollected = RentCollected;
    UE_LOG(LogTemp, Warning, TEXT("house : %d"), HouseData.RentCollected);

    bool bFoundExisting = false;
    for (int32 i = 0; i < SaveGameInstance->HouseDataArray.Num(); ++i)
    {
        if (SaveGameInstance->HouseDataArray[i].HouseID == HouseID)
        {   
            SaveGameInstance->HouseDataArray[i] = HouseData;
            bFoundExisting = true;
            break;
        }
    }

    if (!bFoundExisting)
    {
        SaveGameInstance->HouseDataArray.Add(HouseData);
        UE_LOG(LogTemp, Warning, TEXT("House ID Saved as : %d"), HouseData.HouseID);
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
            if (HouseData.HouseID == HouseID)
            {
                DoesOwnHouse = HouseData.DoesOwnHouse;
                RentCollected = HouseData.RentCollected;
                UE_LOG(LogTemp, Warning, TEXT("Loaded RentCollected: %d"), RentCollected);
                break;
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load save game from slot: HouseSaveSlot or no saved data found"));
    }
}



void AHouseLand::TransferRent()
{
    GM->coins += RentCollected;
    RentCollected = 0;
    UE_LOG(LogTemp, Warning, TEXT("HOUSE TRANSFERRED RENT"));
    SaveGame();
    GM->SaveGame();
}
