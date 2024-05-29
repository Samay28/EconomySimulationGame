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
}
void AFarmLand::BeginPlay()
{
    Super::BeginPlay();
    LoadFarmState();
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
        SaveFarmState();
    }
}
void AFarmLand::SaveFarmState()
{
    UFarmSaveGame* SaveGameInstance = Cast<UFarmSaveGame>(UGameplayStatics::CreateSaveGameObject(UFarmSaveGame::StaticClass()));
    if (SaveGameInstance)
    {
        FFarmData FarmData;
        FarmData.FarmID = FarmID;
        FarmData.bIsRented = bIsRented;
        FarmData.bCropsSowed = bCropsSowed;
        FarmData.HarvestProfit = HarvestProfit;
        FarmData.CropsCost = CropsCost;

        // Remove existing entry with the same FarmID
        for (int32 i = 0; i < SaveGameInstance->FarmDataArray.Num(); ++i)
        {
            if (SaveGameInstance->FarmDataArray[i].FarmID == FarmID)
            {
                SaveGameInstance->FarmDataArray.RemoveAt(i);
                break;
            }
        }

        SaveGameInstance->FarmDataArray.Add(FarmData);

        UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("FarmSaveSlot"), 0);
    }
}

void AFarmLand::LoadFarmState()
{
    UFarmSaveGame* LoadGameInstance = Cast<UFarmSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("FarmSaveSlot"), 0));
    if (LoadGameInstance)
    {
        for (const FFarmData& FarmData : LoadGameInstance->FarmDataArray)
        {
            if (FarmData.FarmID == FarmID)
            {
                bIsRented = FarmData.bIsRented;
                bCropsSowed = FarmData.bCropsSowed;
                HarvestProfit = FarmData.HarvestProfit;
                CropsCost = FarmData.CropsCost;

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
        SaveFarmState();
    }
}
