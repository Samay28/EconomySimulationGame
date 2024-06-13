// Fill out your copyright notice in the Description page of Project Settings.
#include "HouseLand.h"
#include "FarmLand.h"
#include "CarpenterShop.h"
#include "FishShop.h"
#include "OresShop.h"
#include "LandSaveGame.h"
#include "Land.h"
#include "GameManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALand::ALand()
{
    PrimaryActorTick.bCanEverTick = true;
    LandMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Normal Land"));
    LandMesh->SetupAttachment(RootComponent);

    GrassMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Grass"));
    GrassMesh->SetupAttachment(LandMesh);
    Rent = 1;
    LandCost = 50;
    bIsRented = false;

    LandID = FMath::Rand();
    LandTypeNum = 0;
}

void ALand::PurchaseLand()
{
    if (GM->coins >= LandCost && !bIsRented)
    {
        bIsRented = true;
        GrassMesh->SetVisibility(false);
        GM->AddExpenses(Rent);
        LandTypeNum = 0;
        GM->coins -= LandCost;
        SaveGame();
    }
}

void ALand::BeginPlay()
{
    Super::BeginPlay();
    LoadGame();
    AActor *FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass());
    GM = Cast<AGameManager>(FoundActor);

    if (GM && bIsRented)
    {
        GM->AddExpenses(Rent);
        GrassMesh->SetVisibility(false);
        UE_LOG(LogTemp, Warning, TEXT("Hi, %d"), GM->Expenses);
    }
}

void ALand::ConvertToHouse()
{
    if (!HouseLandBlueprint)
    {
        UE_LOG(LogTemp, Warning, TEXT("HouseLandBlueprint is not set."));
        return;
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
        SaveGame();
        Destroy();
    }
}

void ALand::ConvertToCarpenterShop()
{
    if (!CarpenterShopBlueprint)
    {
        UE_LOG(LogTemp, Warning, TEXT("CarpenterShopBlueprint is not set!"));
        return;
    }

    // Spawn the Carpenter Shop Actor
    ACarpenterShop *CarpenterShop = GetWorld()->SpawnActor<ACarpenterShop>(CarpenterShopBlueprint, GetActorLocation(), GetActorRotation());

    if (CarpenterShop)
    {
        // Attach the Carpenter Shop to the LandMesh
        CarpenterShop->AttachToComponent(LandMesh, FAttachmentTransformRules::SnapToTargetIncludingScale);

        // Set transform values
        CarpenterShop->SetActorRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
        CarpenterShop->SetActorRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
        CarpenterShop->SetActorRelativeScale3D(FVector(0.4f, 0.5f, 1.0f));

        UE_LOG(LogTemp, Warning, TEXT("Carpenter Shop successfully converted and attached to LandMesh"));
        LandTypeNum = 3;
        SaveGame();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to spawn Carpenter Shop"));
    }
}

void ALand::ConvertToFishShop()
{
    if (!FishShopBlueprint)
    {
        UE_LOG(LogTemp, Warning, TEXT("SUS"));
        return;
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
        SaveGame();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to spawn Fish Shop"));
    }
}

void ALand::ConvertToOresShop()
{
    if (!OresShopBlueprint)
    {
        UE_LOG(LogTemp, Warning, TEXT("SUS2"));
        return;
    }

    // Spawn the Carpenter Shop Actor
    AOresShop *OresShop = GetWorld()->SpawnActor<AOresShop>(OresShopBlueprint, GetActorLocation(), GetActorRotation());

    if (OresShop)
    {
        // Attach the Carpenter Shop to the LandMesh
        OresShop->AttachToComponent(LandMesh, FAttachmentTransformRules::SnapToTargetIncludingScale);

        // Set transform values
        OresShop->SetActorRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
        OresShop->SetActorRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
        OresShop->SetActorRelativeScale3D(FVector(0.4f, 0.5f, 1.0f));

        UE_LOG(LogTemp, Warning, TEXT("Ores Shop successfully converted and attached to LandMesh"));
        LandTypeNum = 5;
        SaveGame();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to spawn Ores Shop"));
    }
}

void ALand::KeepSimpleLand()
{
    bIsRented = true;
    GrassMesh->SetVisibility(false);
}

void ALand::ConvertToFarm()
{
    if (!FarmLandBlueprint)
    {
        UE_LOG(LogTemp, Warning, TEXT("FarmLandBlueprint is not set."));
        return;
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
        SaveGame();
        Destroy();
    }
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
    SaveData.Rent = Rent;

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
            bIsRented = SaveData.bIsRented; // Add this line
            Rent = SaveData.Rent;           // Add this line

            if (LandTypeNum == 1 && FarmLandBlueprint)
            {
                ConvertToFarm();
            }
            else if (LandTypeNum == 2 && HouseLandBlueprint)
            {
                ConvertToHouse();
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
            break;
        }
    }
}

// Called every frame
void ALand::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ALand::DeductRent()
{
    if (bIsRented)
    {
        GM->coins -= Rent;
    }
}
