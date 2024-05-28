// Fill out your copyright notice in the Description page of Project Settings.

#include "AudioManager.h"

// Sets default values
AAudioManager::AAudioManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	RootComponent = AudioComponent;
	CurrentAudioIndex = 0;
}

void AAudioManager::PlayNextAudioCue()
{
	if (AudioCues.IsValidIndex(CurrentAudioIndex))
	{
		USoundCue *NextCue = AudioCues[CurrentAudioIndex++];
		if (NextCue)
		{
			AudioComponent->OnAudioFinished.Clear(); // Clear existing bindings
			AudioComponent->SetSound(NextCue);
			AudioComponent->Play();
			AudioComponent->OnAudioFinished.AddDynamic(this, &AAudioManager::PlayNextAudioCue);
		}
	}
	else
	{
		CurrentAudioIndex = 0; // Reset the index
		PlayNextAudioCue();	   // Start over
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
