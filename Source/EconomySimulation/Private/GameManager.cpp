
#include "GameManager.h"
#include "EngineUtils.h"
#include "Land.h"
#include "HouseLand.h"
#include "FarmLand.h"
#include "Pond.h"
#include "SaveGameManager.h"
#include "Kismet/GameplayStatics.h"

AGameManager::AGameManager()
{
	PrimaryActorTick.bCanEverTick = true;
	coins = 500;
	BlockPlayerMovement = false;
	IslandValue = 0;
	Profit = 0;
	Expenses = 0;
}




void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	CalculateCoins();
}
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void AGameManager::DeleteFarmSave()
{

	UGameplayStatics::DeleteGameInSlot(TEXT("FarmSaveSlot"), 0);
	UGameplayStatics::DeleteGameInSlot(TEXT("ManagerSaveSlot"), 0);
	UGameplayStatics::DeleteGameInSlot(TEXT("HouseSaveSlot"), 0);
	UGameplayStatics::DeleteGameInSlot(TEXT("LandSaveSlot"), 0);
	UGameplayStatics::DeleteGameInSlot(TEXT("PlayerInventorySlot"), 0);
	UGameplayStatics::DeleteGameInSlot(TEXT("PondSaveSlot"), 0);
	UGameplayStatics::DeleteGameInSlot(TEXT("StorageSlot"), 0);
	UE_LOG(LogTemp, Warning, TEXT("Save game deleted"));
}

void AGameManager::SaveGame()
{
	USaveGameManager *SaveGameInstance = Cast<USaveGameManager>(UGameplayStatics::CreateSaveGameObject(USaveGameManager::StaticClass()));
	if (SaveGameInstance)
	{
		SaveGameInstance->Profit = Profit;
		SaveGameInstance->FirstTimeLoad = FirstTimeLoad;
		UE_LOG(LogTemp, Warning, TEXT("Profit saved as : %d"),SaveGameInstance->Profit );

		UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("ManagerSaveSlot"), 0);
	}
}

void AGameManager::LoadGame()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("ManagerSaveSlot"), 0))
	{
		USaveGameManager *LoadGameInstance = Cast<USaveGameManager>(UGameplayStatics::LoadGameFromSlot(TEXT("ManagerSaveSlot"), 0));
		if (LoadGameInstance)
		{	
			Profit = LoadGameInstance->Profit;
			FirstTimeLoad = LoadGameInstance->FirstTimeLoad;
			UE_LOG(LogTemp, Warning, TEXT("Profit Loaded: %d"),LoadGameInstance->Profit );
		}
	}
}

void AGameManager::CalculateCoins()
{
	coins = Profit - Expenses + 1200;
	// SaveGame();
	UE_LOG(LogTemp, Warning, TEXT("coins : %d"),coins );
}
