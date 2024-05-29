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
	Rent = 1;
	bIsRented = true;
}

// Called when the game starts or when spawned
void ALand::BeginPlay()
{
	Super::BeginPlay();

	AActor *FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass());
	GM = Cast<AGameManager>(FoundActor);

	if (GM)
	{
		GM->AddExpenses(Rent); 
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
