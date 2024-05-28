// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "AudioManager.generated.h"

UCLASS()
class ECONOMYSIMULATION_API AAudioManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAudioManager();

	UPROPERTY(EditAnywhere, Category = "Audio")
	TArray<USoundCue *> AudioCues;

	UPROPERTY(VisibleAnywhere, Category = "Audio")
	UAudioComponent *AudioComponent;

	int32 CurrentAudioIndex;

	UFUNCTION()
	void PlayNextAudioCue();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
