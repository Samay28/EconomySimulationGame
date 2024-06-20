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
    HouseMesh->SetVisibility(false);

    HouseConstructionCost = 50;
    bIsRented = false;
    DoesOwnHouse = false;
    PayCheck = 10;
    count = 0;
}

void AHouseLand::BeginPlay()
{
    Super::BeginPlay();
    LoadGame();
    OwnHouse();
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
        bIsRented = true;
        GM->coins -= HouseConstructionCost;
        UE_LOG(LogTemp, Warning, TEXT("House Bought, Remaining money : %d"), GM->coins);
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

    // Aggregate the rent from all instances
    // int32 TotalRentCollected = 0;
    // for (TActorIterator<AHouseLand> It(GetWorld()); It; ++It)
    // {
    //     AHouseLand* House = *It;
    //     RentCollected += House->RentCollected;
    // }

    FHouseData HouseData;
    HouseData.DoesOwnHouse = DoesOwnHouse;
    HouseData.PayCheck = PayCheck;
    HouseData.RentCollected = RentCollected;
    HouseData.count = count;
    UE_LOG(LogTemp, Warning, TEXT("Total Rent Saved As : %d"), HouseData.RentCollected);

    // Add or update the house data in the array
    SaveGameInstance->HouseDataArray.Empty();
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
    if (LoadGameInstance && LoadGameInstance->HouseDataArray.Num() > 0)
    {
        const FHouseData &HouseData = LoadGameInstance->HouseDataArray[0];
        DoesOwnHouse = HouseData.DoesOwnHouse;
        PayCheck = HouseData.PayCheck;
        count = HouseData.count;
        RentCollected = HouseData.RentCollected;
        HouseMesh->SetVisibility(DoesOwnHouse);
        UE_LOG(LogTemp, Warning, TEXT("Loaded RentCollected: %d"), RentCollected);
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

    SaveGame();
    GM->SaveGame();
}
