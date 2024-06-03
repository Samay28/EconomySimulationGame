// Fill out your copyright notice in the Description page of Project Settings.

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
