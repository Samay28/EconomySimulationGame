
#include "GameManager.h"
#include "EngineUtils.h"
#include "Land.h"
#include "HouseLand.h"
#include "FarmLand.h"
#include "SaveGameManager.h"
#include "Kismet/GameplayStatics.h"

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
	LoadAll();
}
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void AGameManager::TriggerDailyEconomy()
{
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
	for (ALand *LandActor : LandActors)
	{
		if (LandActor)
		{
			LandActor->DeductRent();
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

	UGameplayStatics::DeleteGameInSlot(TEXT("FarmSaveSlot"), 0);
	UGameplayStatics::DeleteGameInSlot(TEXT("ManagerSaveSlot"), 0);
	UGameplayStatics::DeleteGameInSlot(TEXT("HouseSaveSlot"), 0);
	UGameplayStatics::DeleteGameInSlot(TEXT("LandSaveSlot"), 0);
	UE_LOG(LogTemp, Warning, TEXT("Save game deleted"));
}

void AGameManager::SaveGame()
{
	USaveGameManager *SaveGameInstance = Cast<USaveGameManager>(UGameplayStatics::CreateSaveGameObject(USaveGameManager::StaticClass()));
	if (SaveGameInstance)
	{
		SaveGameInstance->Coins = coins;
		SaveGameInstance->Expenses = Expenses;
		SaveGameInstance->Income = Income;

		UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("ManagerSaveSlot"), 0);
		UE_LOG(LogTemp, Warning, TEXT("Game saved"));
	}
}

void AGameManager::LoadGame()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("ManagerSaveSlot"), 0))
	{
		USaveGameManager *LoadGameInstance = Cast<USaveGameManager>(UGameplayStatics::LoadGameFromSlot(TEXT("ManagerSaveSlot"), 0));
		if (LoadGameInstance)
		{
			coins = LoadGameInstance->Coins;
			Expenses = LoadGameInstance->Expenses;
			Income = LoadGameInstance->Income;
			UE_LOG(LogTemp, Warning, TEXT("Game loaded"));
		}
	}
}

void AGameManager::SaveAll()
{
	TArray<AActor *> SaveableActors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USaveGameInterface::StaticClass(), SaveableActors);

	for (AActor *Actor : SaveableActors)
	{
		ISaveGameInterface *SaveableActor = Cast<ISaveGameInterface>(Actor);
		if (SaveableActor)
		{
			SaveableActor->SaveGame();
		}
	}
}

void AGameManager::LoadAll()
{
	TArray<AActor *> SaveableActors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USaveGameInterface::StaticClass(), SaveableActors);

	for (AActor *Actor : SaveableActors)
	{
		ISaveGameInterface *SaveableActor = Cast<ISaveGameInterface>(Actor);
		if (SaveableActor)
		{
			SaveableActor->LoadGame();
		}
	}
}