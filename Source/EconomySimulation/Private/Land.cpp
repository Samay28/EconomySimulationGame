#include "HouseLand.h"
#include "FarmLand.h"
#include "Pond.h"
#include "MiningLand.h"
#include "CarpenterShop.h"
#include "FishShop.h"
#include "OresShop.h"
#include "VegetableShop.h"
#include "LandSaveGame.h"
#include "Land.h"
#include "GameManager.h"
#include "MyPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

bool ALand::IsStoragePresent = false;
bool ALand::IsVegetableShopPresent = false;
bool ALand::IsATMPresent = false;
bool ALand::IsFishShopPresent = false;
bool ALand::IsOreShopPresent = false;
bool ALand::AreItemsReady = false;
bool ALand::IsRentReady = false;

ALand::ALand()
{
    PrimaryActorTick.bCanEverTick = true;
    LandMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Normal Land"));
    LandMesh->SetupAttachment(RootComponent);

    GrassMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Grass"));
    GrassMesh->SetupAttachment(LandMesh);
    LandCost = 200;
    bIsRented = false;
    IsOccupied = false;
    AreItemsReady = false;

    LandTypeNum = 0;
    LandID = FMath::Rand();
}

bool ALand::PurchaseLand()
{
    if (GM->coins >= LandCost && !bIsRented)
    {
        bIsRented = true;
        GrassMesh->SetVisibility(false);
        LandTypeNum = 0;
        GM->Expenses += LandCost;
        GM->IslandValue += 100;
        SaveGame();
        GM->CalculateCoins();
        GM->SaveGame();
        return true;
    }
    else
    {
        return false;
    }
}

void ALand::BeginPlay()
{
    Super::BeginPlay();
    AActor *FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass());
    GM = Cast<AGameManager>(FoundActor);

    LoadGame();
    if (GM && bIsRented)
    {
        KeepSimpleLand();
    }
}

bool ALand::ConvertToHouse()
{
    if (!HouseLandBlueprint || GM->coins <=500)
    {   
        UE_LOG(LogTemp, Warning, TEXT("coins : %d"),GM->coins );
        return false;
    }

    FVector Location = GetActorLocation();
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = GetInstigator();

    AHouseLand *NewHouse = GetWorld()->SpawnActor<AHouseLand>(HouseLandBlueprint, Location, FRotator::ZeroRotator, SpawnParams);
    if (NewHouse)
    {
        CreatedActors.Add(NewHouse);
        LandTypeNum = 2;
        GM->IslandValue += 200;
        IsOccupied = true;
        SaveGame();
        Destroy();
    }
    return true;
}

bool ALand::ConvertToPond()
{
    if (!PondBlueprint || !IsFishShopPresent || GM->coins <=800)
    {
        return false;
    }

    FVector Location = GetActorLocation();
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = GetInstigator();

    APond *NewPond = GetWorld()->SpawnActor<APond>(PondBlueprint, Location, FRotator::ZeroRotator, SpawnParams);
    if (NewPond)
    {
        CreatedActors.Add(NewPond);
        LandTypeNum = 6;
        GM->IslandValue += 250;
        IsOccupied = true;
        SaveGame();
        Destroy();
    }
    return true;
}

bool ALand::ConvertToMiningLand()
{
    if (!MiningLandBlueprint || !IsOreShopPresent || GM->coins <=1200)
    {
        return false;
    }

    FVector Location = GetActorLocation();
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = GetInstigator();

    AMiningLand *NewMiningLand = GetWorld()->SpawnActor<AMiningLand>(MiningLandBlueprint, Location, FRotator::ZeroRotator, SpawnParams);
    if (NewMiningLand)
    {
        CreatedActors.Add(NewMiningLand);
        GM->IslandValue += 350;
        LandTypeNum = 7;
        IsOccupied = true;
        SaveGame();
        Destroy();
    }
    return true;
}

bool ALand::ConvertToCarpenterShop()
{
    if (!CarpenterShopBlueprint)
    {
        return false;
    }

    ACarpenterShop *CarpenterShop = GetWorld()->SpawnActor<ACarpenterShop>(CarpenterShopBlueprint, GetActorLocation(), GetActorRotation());

    if (CarpenterShop)
    {

        CarpenterShop->AttachToComponent(LandMesh, FAttachmentTransformRules::SnapToTargetIncludingScale);

        CarpenterShop->SetActorRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
        CarpenterShop->SetActorRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
        CarpenterShop->SetActorRelativeScale3D(FVector(0.4f, 0.5f, 1.0f));

        UE_LOG(LogTemp, Warning, TEXT("Carpenter Shop successfully converted and attached to LandMesh"));
        LandTypeNum = 3;
        GM->IslandValue += 50;
        IsOccupied = true;
        SaveGame();
    }
    return true;
}

bool ALand::ConvertToFishShop()
{
    if (!FishShopBlueprint)
    {
        return false;
    }

    // Spawn the Carpenter Shop Actor
    AFishShop *FishShop = GetWorld()->SpawnActor<AFishShop>(FishShopBlueprint, GetActorLocation(), GetActorRotation());

    if (FishShop)
    {
        // Attach the Carpenter Shop to the LandMesh
        FishShop->AttachToComponent(LandMesh, FAttachmentTransformRules::SnapToTargetIncludingScale);

        // Set transform values
        FishShop->SetActorRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
        FishShop->SetActorRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
        FishShop->SetActorRelativeScale3D(FVector(0.4f, 0.5f, 1.0f));

        UE_LOG(LogTemp, Warning, TEXT("Fish Shop successfully converted and attached to LandMesh"));
        LandTypeNum = 4;
        GM->IslandValue += 50;
        IsFishShopPresent = true;
        IsOccupied = true;
        SaveGame();
    }
    return true;
}

bool ALand::ConvertToOresShop()
{
    if (!OresShopBlueprint)
    {
        return false;
    }
    AOresShop *OresShop = GetWorld()->SpawnActor<AOresShop>(OresShopBlueprint, GetActorLocation(), GetActorRotation());

    if (OresShop)
    {
        OresShop->AttachToComponent(LandMesh, FAttachmentTransformRules::SnapToTargetIncludingScale);

        OresShop->SetActorRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
        OresShop->SetActorRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
        OresShop->SetActorRelativeScale3D(FVector(0.4f, 0.5f, 1.0f));

        UE_LOG(LogTemp, Warning, TEXT("Ores Shop successfully converted and attached to LandMesh"));
        LandTypeNum = 5;
        GM->IslandValue += 50;
        IsOreShopPresent = true;
        IsOccupied = true;
        SaveGame();
    }
    return true;
}

bool ALand::ConvertToVegetableShop()
{
    if (!VegetableShopBlueprint)
    {
        return false;
    }

    // Spawn the Carpenter Shop Actor
    AVegetableShop *VegetableShop = GetWorld()->SpawnActor<AVegetableShop>(VegetableShopBlueprint, GetActorLocation(), GetActorRotation());
    IsVegetableShopPresent = true;
    if (VegetableShop)
    {
        // Attach the Carpenter Shop to the LandMesh
        VegetableShop->AttachToComponent(LandMesh, FAttachmentTransformRules::SnapToTargetIncludingScale);

        VegetableShop->SetActorRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
        VegetableShop->SetActorRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
        VegetableShop->SetActorRelativeScale3D(FVector(0.4f, 0.5f, 1.0f));
        LandTypeNum = 8;
        GM->IslandValue += 50;
        IsOccupied = true;
        SaveGame();
    }
    return true;
}

void ALand::KeepSimpleLand()
{
    GM->LoadGame();
    GM->IslandValue += 100;
    GrassMesh->SetVisibility(false);
    GM->Expenses += LandCost;
    GM->CalculateCoins();
}

bool ALand::ConvertToFarm()
{
    if (!IsVegetableShopPresent || GM->coins <=300)
    {
        return false;
    }

    FVector Location = GetActorLocation();
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = GetInstigator();

    AFarmLand *NewFarm = GetWorld()->SpawnActor<AFarmLand>(FarmLandBlueprint, Location, FRotator::ZeroRotator, SpawnParams);
    if (NewFarm)
    {
        CreatedActors.Add(NewFarm);
        LandTypeNum = 1;
        GM->IslandValue += 100;
        IsOccupied = true;
        SaveGame();
        Destroy();
    }
    return true;
}

void ALand::SaveGame()
{
    ULandSaveGame *SaveGameInstance = Cast<ULandSaveGame>(UGameplayStatics::CreateSaveGameObject(ULandSaveGame::StaticClass()));
    if (!SaveGameInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to create SaveGameInstance"));
        return;
    }

    FLandSaveData SaveData;
    SaveData.LandID = LandID;
    SaveData.LandTypeNum = LandTypeNum;
    SaveData.Location = GetActorLocation();
    SaveData.Rotation = GetActorRotation();
    SaveData.bIsRented = bIsRented;

    if (UGameplayStatics::DoesSaveGameExist(TEXT("LandSaveSlot"), 0))
    {
        SaveGameInstance = Cast<ULandSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("LandSaveSlot"), 0));
    }

    for (int32 i = 0; i < SaveGameInstance->LandDataArray.Num(); ++i)
    {
        if (SaveGameInstance->LandDataArray[i].LandID == LandID)
        {
            SaveGameInstance->LandDataArray.RemoveAt(i);
            break;
        }
    }

    SaveGameInstance->LandDataArray.Add(SaveData);
    UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("LandSaveSlot"), 0);
    UE_LOG(LogTemp, Warning, TEXT("Game Saved"));
    // GM->SaveGame();
}

void ALand::LoadGame()
{
    if (!UGameplayStatics::DoesSaveGameExist(TEXT("LandSaveSlot"), 0))
    {
        UE_LOG(LogTemp, Warning, TEXT("No save game found"));
        return;
    }

    ULandSaveGame *LoadGameInstance = Cast<ULandSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("LandSaveSlot"), 0));
    if (!LoadGameInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load save game"));
        return;
    }

    for (const FLandSaveData &SaveData : LoadGameInstance->LandDataArray)
    {
        if (SaveData.LandID == LandID)
        {
            LandTypeNum = SaveData.LandTypeNum;
            SetActorLocation(SaveData.Location);
            SetActorRotation(SaveData.Rotation);
            bIsRented = SaveData.bIsRented;

            if (LandTypeNum == 1 && FarmLandBlueprint)
            {
                ConvertToFarm();
                // GM->IslandValue += 100;
                // GM->Expenses += LandCost;
                GM->CalculateCoins();
            }
            else if (LandTypeNum == 2 && HouseLandBlueprint)
            {
                ConvertToHouse();
                // GM->IslandValue += 100;
                // GM->Expenses += LandCost;
                GM->CalculateCoins();
            }
            else if (LandTypeNum == 3 && CarpenterShopBlueprint)
            {
                ConvertToCarpenterShop();

            }
            else if (LandTypeNum == 4 && FishShopBlueprint)
            {
                ConvertToFishShop();

            }
            else if (LandTypeNum == 5 && OresShopBlueprint)
            {
                ConvertToOresShop();

            }
            else if (LandTypeNum == 6 && PondBlueprint)
            {
                ConvertToPond();
                // GM->IslandValue += 100;
                // GM->Expenses += LandCost;
                GM->CalculateCoins();
            }
            else if (LandTypeNum == 7 && MiningLandBlueprint)
            {
                ConvertToMiningLand();
                // GM->IslandValue += 100;
                // GM->Expenses += LandCost;
                GM->CalculateCoins();
            }
            else if (LandTypeNum == 8 && VegetableShopBlueprint)
            {
                ConvertToVegetableShop();
            }
            break;
        }
    }
}

bool ALand::GetAreItemsReady()
{
    return AreItemsReady;
}

bool ALand::GetIsRentReady()
{
    return IsRentReady;
}

// Called every frame
void ALand::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}