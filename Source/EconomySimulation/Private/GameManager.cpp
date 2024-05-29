
#include "GameManager.h"
#include "EngineUtils.h"
#include "Land.h"
#include "HouseLand.h"
#include "FarmLand.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AGameManager::AGameManager()
{
	PrimaryActorTick.bCanEverTick = true;
	coins = 500;
	Expenses = 0;
	Income = 0;
	BlockPlayerMovement = false;
}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	for (TActorIterator<ALand> It(GetWorld()); It; ++It)
	{
		LandActors.Add(*It);
	}

	for (TActorIterator<AHouseLand> It(GetWorld()); It; ++It)
	{
		HouseActors.Add(*It);
	}

	for (TActorIterator<AFarmLand> It(GetWorld()); It; ++It)
	{
		FarmActors.Add(*It);
	}
	GetWorldTimerManager().SetTimer(DayNightCycle, this, &AGameManager::TriggerDailyEconomy, 300.f, true);
}
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void AGameManager::TriggerDailyEconomy()
{
	for (ALand *LandActor : LandActors)
	{
		if (LandActor)
		{
			LandActor->DeductRent();
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("%d"), coins);
	// Update each HouseActor
	for (AHouseLand *HouseActor : HouseActors)
	{
		if (HouseActor)
		{
			HouseActor->GetHouseRent();
		}
	}

	// Update each FarmActor
	for (AFarmLand *FarmActor : FarmActors)
	{
		if (FarmActor)
		{
			FarmActor->HarvestCrops();
		}
	}
}
// Called every frame

void AGameManager::AddExpenses(int Amount)
{
	Expenses += Amount;
}

void AGameManager::AddIncome(int Amount)
{
	Income += Amount;
}
void AGameManager::DeleteFarmSave()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("FarmSaveSlot"), 0))
	{
		UGameplayStatics::DeleteGameInSlot(TEXT("FarmSaveSlot"), 0);
		UE_LOG(LogTemp, Warning, TEXT("Save game deleted"));
	}
}
