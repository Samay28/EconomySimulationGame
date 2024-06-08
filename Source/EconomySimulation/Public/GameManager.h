// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveGameInterface.h"
#include "GameManager.generated.h"

UCLASS()
class ECONOMYSIMULATION_API AGameManager : public AActor, public ISaveGameInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGameManager();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int coins;

	UPROPERTY(BlueprintReadOnly)
	int Expenses;

	UPROPERTY(BlueprintReadOnly)
	int Income;

	virtual void Tick(float DeltaTime) override;

	void AddExpenses(int Amount);
	void AddIncome(int Amount);

	UPROPERTY(BlueprintReadWrite)
	bool BlockPlayerMovement;

	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	void DeleteFarmSave();

	virtual void SaveGame() override;
	virtual void LoadGame() override;

	UFUNCTION(BlueprintCallable)
	void SaveAll();
	UFUNCTION(BlueprintCallable)
	void LoadAll();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	FTimerHandle DayNightCycle;

	void TriggerDailyEconomy();

	TArray<class ALand *> LandActors;
	TArray<class AFarmLand *> FarmActors;
	TArray<class AHouseLand *> HouseActors;
};
