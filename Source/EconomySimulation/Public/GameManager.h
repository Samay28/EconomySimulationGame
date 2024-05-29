// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	int Expenses;

	UPROPERTY(BlueprintReadOnly)
	int Income;

	void AddExpenses(int Amount);
	void AddIncome(int Amount);

	bool BlockPlayerMovement;

	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	void DeleteFarmSave();

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
