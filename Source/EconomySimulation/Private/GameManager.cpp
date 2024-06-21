
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
	IslandValue = 0;
	BlockPlayerMovement = false;
}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	LoadGame();
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
		SaveGameInstance->Coins = coins;
		SaveGameInstance->IslandValue = IslandValue;
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("ManagerSaveSlot"), 0);
		UE_LOG(LogTemp, Warning, TEXT("GAME MANAGER Game saved"));
	}
}

void AGameManager::LoadGame()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("ManagerSaveSlot"), 0))
	{
		USaveGameManager *LoadGameInstance = Cast<USaveGameManager>(UGameplayStatics::LoadGameFromSlot(TEXT("ManagerSaveSlot"), 0));
		if (LoadGameInstance)
		{	
			IslandValue = LoadGameInstance->IslandValue;
			coins = LoadGameInstance->Coins;
			UE_LOG(LogTemp, Warning, TEXT("Game loaded"));
		}
	}
}
