// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Land.generated.h"

UCLASS()
class ECONOMYSIMULATION_API ALand : public AActor
{
	GENERATED_BODY()

public:

	ALand();

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UStaticMeshComponent* LandMesh;

	UPROPERTY(EditAnywhere)
	int Rent;

	virtual void Tick(float DeltaTime) override;

	void DeductRent();

	bool bIsRented;

	class AGameManager* GM;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
