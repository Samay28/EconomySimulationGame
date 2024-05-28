// Fill out your copyright notice in the Description page of Project Settings.

#include "HouseLand.h"
#include "GameManager.h"
#include "Math/UnrealMathUtility.h"

AHouseLand::AHouseLand()
{
    HouseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("House Mesh"));
    HouseMesh->SetupAttachment(LandMesh);
    HouseMesh->SetVisibility(false);

    HouseConstructionCost = 50;
    bIsRented = false;
    DoesOwnHouse = false;
    PayCheck = 30;
    MaintenanceFees = 10;
}
void AHouseLand::BeginPlay()
{
    Super::BeginPlay();
}
void AHouseLand::GetHouseRent()
{
    if (DoesOwnHouse)
    {   
        PayCheck = FMath::RandRange(5,45);
        UE_LOG(LogTemp, Warning, TEXT("Credited House Rent"));
        GM->coins-=MaintenanceFees;
        GM->coins+=PayCheck;
    }
}
void AHouseLand::OwnHouse()
{
    if (GM->coins >= HouseConstructionCost && !DoesOwnHouse)
    {   
        PayCheck = FMath::RandRange(5,45);
        DoesOwnHouse = true;
        HouseMesh->SetVisibility(true);
        bIsRented = true;
        GM->coins -= HouseConstructionCost;
        UE_LOG(LogTemp,Warning,TEXT("House Bought, Remaining money : %d"),GM->coins);
        GM->AddIncome(PayCheck);
        GM->AddExpenses(MaintenanceFees);
    }
}
void AHouseLand::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}