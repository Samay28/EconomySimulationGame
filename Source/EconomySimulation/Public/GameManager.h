// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveGameInterface.h"
#include "GameManager.generated.h"

UCLASS()
class ECONOMYSIMULATION_API AGameManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGameManager();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int coins;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Profit;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Expenses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int IslandValue;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite)
	bool BlockPlayerMovement;

	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	void DeleteFarmSave();

	UFUNCTION(BlueprintCallable)
	void SaveGame();

	UFUNCTION(BlueprintCallable)
	void LoadGame();

	void CalculateCoins();

	UPROPERTY(BlueprintReadWrite)
	bool FirstTimeLoad;

protected:
	
	virtual void BeginPlay() override;

private:
};
