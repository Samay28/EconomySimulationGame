#include "AudioManager.h"

// Sets default values
AAudioManager::AAudioManager()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
    RootComponent = AudioComponent;
}

void AAudioManager::PlayNextAudioCue()
{
    if (AudioCues.Num() > 0)
    {
        CurrentAudioIndex = FMath::RandRange(0, AudioCues.Num() - 1);
        
        USoundCue *NextCue = AudioCues[CurrentAudioIndex];
        if (NextCue)
        {
            AudioComponent->OnAudioFinished.Clear(); // Clear existing bindings
            AudioComponent->SetSound(NextCue);
            AudioComponent->Play();
            AudioComponent->OnAudioFinished.AddDynamic(this, &AAudioManager::PlayNextAudioCue);
        }
    }
}

// Called when the game starts or when spawned
void AAudioManager::BeginPlay()
{
    Super::BeginPlay();
    PlayNextAudioCue();
}

// Called every frame
void AAudioManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
