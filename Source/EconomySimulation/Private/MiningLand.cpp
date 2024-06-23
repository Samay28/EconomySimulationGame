
#include "MiningLand.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayerCharacter.h"
#include "BusinessStorageComponent.h"
#include "PondSaveGame.h"
#include "Math/UnrealMathUtility.h"
#include "GameManager.h"

AMiningLand::AMiningLand()
{
    MineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mining Mesh"));
    MineMesh->SetupAttachment(LandMesh);

    StorageComponent = CreateDefaultSubobject<UBusinessStorageComponent>(TEXT("Storage Component"));

    MiningLandSetupCost = 50;
    IsPurchased = false;
    count = 0;
    MiningLevel = 1;
}

void AMiningLand::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMiningLand::BeginPlay()
{
    Super::BeginPlay();
    if (count == 0)
    {
        SetupMiningLand();
    }
    GetWorldTimerManager().SetTimer(ItemsProvider, this, &AMiningLand::ProvideResourcesToStorage, 10.0f, true);
    APlayerController *PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    Player = Cast<AMyPlayerCharacter>(PlayerController->GetCharacter());
}

void AMiningLand::TransferItems()
{
    if (Player && Player->PlayerInventoryComponent && StorageComponent)
    {
        for (const auto &Item : StorageComponent->GetItems())
        {
            Player->PlayerInventoryComponent->AddItem(Item.ItemName, Item.Quantity, Item.Value);
            StorageComponent->RemoveItem(Item.ItemName, Item.Quantity);
            UE_LOG(LogTemp, Warning, TEXT("TRANSFERRING Name: %s, Quantity: %d, Value : %d"), *Item.ItemName.ToString(), Item.Quantity, Item.Value);
            StorageComponent->SaveStorage();
        }
    }
    // StorageComponent->ClearItems();
}

void AMiningLand::SetupMiningLand()
{
    if (GM->coins >= MiningLandSetupCost && !IsPurchased)
    {
        MiningLevel = 1;
        GM->coins -= MiningLandSetupCost;
        count++;
    }
}

void AMiningLand::ProvideResourcesToStorage()
{
    const float CoalProbability = 0.6f;
    const float CopperProbability = 0.3f;
    const float IronProbability = 0.1f;

    float RandomValue = FMath::FRand();

    FName AwardedOre;
    int Value;
    if (RandomValue < CoalProbability)
    {
        AwardedOre = "coal";
        Value = 30;
    }
    else if (RandomValue < CoalProbability + CopperProbability)
    {
        AwardedOre = "copper";
        Value = 40;
    }
    else
    {
        AwardedOre = "iron";
        Value = 55;
    }
    int32 Quantity = FMath::RandRange(1, 2);
    StorageComponent->AddItem(AwardedOre, Quantity, Value);

    UE_LOG(LogTemp, Warning, TEXT("Harvested: %s, Quantity: %d"), *AwardedOre.ToString(), Quantity);
    StorageComponent->SaveStorage();
}

void AMiningLand::SaveGame()
{
}

void AMiningLand::LoadGame()
{
}
