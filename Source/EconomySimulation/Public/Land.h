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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent *LandMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent *GrassMesh;

	UPROPERTY(EditAnywhere)
	int Rent;

	UPROPERTY(EditAnywhere)
	bool bIsRented;
	int LandCost;
	
	void DeductRent();

	class AGameManager *GM;

	UFUNCTION(BlueprintCallable)
	void PurchaseLand();

	UFUNCTION(BlueprintCallable)
	void ConvertToFarm();

	UFUNCTION(BlueprintCallable)
	void ConvertToHouse();
	TArray<AActor*> CreatedActors;

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
};
