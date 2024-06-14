// Fill out your copyright notice in the Description page of Project Settings.

#include "Pond.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayerCharacter.h"
#include "Math/UnrealMathUtility.h"
#include "GameManager.h"

APond::APond()
{
    WaterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WaterMesh"));
    WaterMesh->SetupAttachment(LandMesh);

    PondSetupCost = 50;
    IsPurchased = true;
    CleaningCost = 10;
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
        GM->AddExpenses(CleaningCost);
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

    UE_LOG(LogTemp, Warning, TEXT("trout : %d, tuna : %d, salmon : %d"), TroutAwardedTemp, TunaAwardedTemp, SalmonAwardedTemp);
}

void APond::SaveGame()
{
}

void APond::LoadGame()
{
}

