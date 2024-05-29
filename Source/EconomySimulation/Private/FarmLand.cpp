// Fill out your copyright notice in the Description page of Project Settings.

#include "FarmLand.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "GameManager.h"

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
}
void AFarmLand::BeginPlay()
{
    Super::BeginPlay();
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
