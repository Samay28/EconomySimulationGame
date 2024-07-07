#include "Tree.h"
#include "GameManager.h"
#include "MyPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ATree::ATree()
{
    PrimaryActorTick.bCanEverTick = true;
    TreeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tree Mesh"));
    TreeMesh->SetupAttachment(RootComponent);

    LandForTreeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TreeLandMesh"));
    LandForTreeMesh->SetupAttachment(TreeMesh);

    TimeToCut = 10;
    TimeToRegenerate = 60;
    CanBeCut = true;
    IsCutting = false; // Initialize IsCutting flag
}

void ATree::BeginPlay()
{
    Super::BeginPlay();
    AActor *FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass());
    GM = Cast<AGameManager>(FoundActor);

    APlayerController *PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    Player = Cast<AMyPlayerCharacter>(PlayerController->GetCharacter());
    InitialRotation = GetActorRotation();
}

void ATree::StartCuttingTrees()
{
    GetWorldTimerManager().SetTimer(RewardTimerHandle, this, &ATree::ProvideRewards, TimeToCut, false);
    if (CanBeCut)
    {
        IsCutting = true; // Set IsCutting to true
        GetWorldTimerManager().SetTimer(RewardTimerHandle, this, &ATree::ProvideRewards, TimeToCut, false);
        GM->BlockPlayerMovement = true;
    }
}

void ATree::ProvideRewards()
{

    const float LeafProbability = 0.6f;
    const float WoodProbability = 0.3f;
    const float StoneProbability = 0.1f;

    float RandomValue = FMath::FRand();
    FName AwardedItem;
    int Value;
    if (RandomValue < LeafProbability)
    {
        AwardedItem = "leaf";
        Value = 2;
    }
    else if (RandomValue < LeafProbability + WoodProbability)
    {
        AwardedItem = "wood";
        Value = 5;
    }
    else
    {
        AwardedItem = "stone";
        Value = 7;
    }
    int Quantity = FMath::RandRange(1, 3);
    Player->PlayerInventoryComponent->AddItem(AwardedItem, Quantity, Value);
    TreeMesh->SetVisibility(false);
    CanBeCut = false;
    IsCutting = false;
    GetWorldTimerManager().SetTimer(RegenerationTimerHandle, this, &ATree::RegenerateTime, TimeToRegenerate, false);
    GM->BlockPlayerMovement = false;
}

void ATree::RegenerateTime()
{
    TreeMesh->SetVisibility(true);
    CanBeCut = true;
    SetActorRotation(InitialRotation);
}

void ATree::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
