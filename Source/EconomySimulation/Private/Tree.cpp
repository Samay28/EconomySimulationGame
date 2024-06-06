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

    // coinsToBeRewarded = 10;
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
    UE_LOG(LogTemp, Warning, TEXT("fwfwfwf"));
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
    TreeMesh->SetVisibility(false);
    int32 woodAwardedTemp = FMath::RandRange(10, 15);
    Player->PlayerInventoryComponent->AddItem("wood", woodAwardedTemp);
    UE_LOG(LogTemp, Warning, TEXT("Tree cut! Coins rewarded: %d"), woodAwardedTemp);
    CanBeCut = false;
    IsCutting = false; // Stop the cutting effect
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

    if (IsCutting)
    {
        const float Angle = FMath::Sin(GetWorld()->TimeSeconds * 50.0f) * 3.0f; // Fast sine wave to shake
        FRotator NewRotation = GetActorRotation();
        NewRotation.Pitch = Angle;
        SetActorRotation(NewRotation);
    }
}
