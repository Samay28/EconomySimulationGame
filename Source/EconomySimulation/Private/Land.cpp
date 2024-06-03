// Fill out your copyright notice in the Description page of Project Settings.
#include "HouseLand.h"
#include "FarmLand.h"
#include "Land.h"
#include "GameManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALand::ALand()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	LandMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Normal Land"));
	LandMesh->SetupAttachment(RootComponent);

	GrassMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Grass"));
	GrassMesh->SetupAttachment(LandMesh);
	Rent = 1;
	LandCost = 50;
	bIsRented = false;
}

void ALand::PurchaseLand()
{
	if (GM->coins >= LandCost && !bIsRented)
	{
		bIsRented = true;
		GrassMesh->SetVisibility(false);
		GM->AddExpenses(Rent);
		GM->coins -= LandCost;
	}
}

void ALand::BeginPlay()
{
	Super::BeginPlay();

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
		Destroy(); // Destroy the current land actor
	}
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
		Destroy(); // Destroy the current land actor
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
