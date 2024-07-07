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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsRented;

	int LandCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsOccupied;

	UPROPERTY(BlueprintReadOnly)
	class AGameManager *GM;
	class AMyPlayerCharacter *Player;

	UFUNCTION(BlueprintCallable)
	bool PurchaseLand();
	static bool IsStoragePresent;

	UFUNCTION(BlueprintCallable)
	bool ConvertToFarm();
	static bool IsVegetableShopPresent;

	UFUNCTION(BlueprintCallable)
	bool ConvertToHouse();
	static bool IsATMPresent;

	UFUNCTION(BlueprintCallable)
	bool ConvertToPond();
	static bool IsFishShopPresent;

	UFUNCTION(BlueprintCallable)
	bool ConvertToMiningLand();
	static bool IsOreShopPresent;

	UFUNCTION(BlueprintCallable)
	bool ConvertToCarpenterShop();

	UFUNCTION(BlueprintCallable)
	bool ConvertToFishShop();

	UFUNCTION(BlueprintCallable)
	bool ConvertToOresShop();

	UFUNCTION(BlueprintCallable)
	bool ConvertToVegetableShop();

	UFUNCTION(BlueprintCallable)
	void KeepSimpleLand();

	UPROPERTY(EditDefaultsOnly, Category = "Conversion")
	TSubclassOf<class AHouseLand> HouseLandBlueprint;

	UPROPERTY(EditDefaultsOnly, Category = "Conversion")
	TSubclassOf<class AFarmLand> FarmLandBlueprint;

	UPROPERTY(EditDefaultsOnly, Category = "Conversion")
	TSubclassOf<class APond> PondBlueprint;

	UPROPERTY(EditDefaultsOnly, Category = "Conversion")
	TSubclassOf<class AMiningLand> MiningLandBlueprint;

	UPROPERTY(EditDefaultsOnly, Category = "Conversion")
	TSubclassOf<class ACarpenterShop> CarpenterShopBlueprint;

	UPROPERTY(EditAnywhere, Category = "Conversion")
	TSubclassOf<class AFishShop> FishShopBlueprint;

	UPROPERTY(EditAnywhere, Category = "Conversion")
	TSubclassOf<class AOresShop> OresShopBlueprint;

	UPROPERTY(EditAnywhere, Category = "Conversion")
	TSubclassOf<class AVegetableShop> VegetableShopBlueprint;

	UPROPERTY()
	TArray<AActor *> CreatedActors;

	UFUNCTION(BlueprintCallable)
	void SaveGame();

	UFUNCTION(BlueprintCallable)
	void LoadGame();

	// printing UI

	static bool AreItemsReady;
	static bool IsRentReady;

	UFUNCTION(BlueprintCallable)
	bool GetAreItemsReady();

	UFUNCTION(BlueprintCallable)
	bool GetIsRentReady();

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	int LandTypeNum;

private:
	UPROPERTY(EditAnywhere)
	int32 LandID;
};
