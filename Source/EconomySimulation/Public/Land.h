// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Land.generated.h"

UENUM(BlueprintType)
enum class EMessageType : uint8
{
	MT_ErrorSettingItemProivderLand UMETA(DisplayName = "Shop/storage absent"),
	MT_ErrorSettingProfitProivderLand UMETA(DisplayName = "Atm Absent"),
	MT_InsufficientFunds UMETA(DisplayName = "Insufficient Funds"),
	MT_SuccessfullPurchase UMETA(DisplayName = "Purchase Done"),
	MT_GameLoaded UMETA(DisplayName = "Game Loaded")
};

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

	void DeductRent();

	UPROPERTY(BlueprintReadOnly)
	class AGameManager *GM;
	class AMyPlayerCharacter *Player;

	UFUNCTION(BlueprintCallable)
	void PurchaseLand();
	bool IsStoragePresent;

	UFUNCTION(BlueprintCallable)
	bool ConvertToFarm();
	bool IsVegetableShopPresent;

	UFUNCTION(BlueprintCallable)
	void ConvertToHouse();
	bool IsATMPresent;

	UFUNCTION(BlueprintCallable)
	void ConvertToPond();
	bool IsFishShopPresent;

	UFUNCTION(BlueprintCallable)
	void ConvertToMiningLand();
	bool IsOreShopPresent;

	UFUNCTION(BlueprintCallable)
	void ConvertToCarpenterShop();

	UFUNCTION(BlueprintCallable)
	void ConvertToFishShop();

	UFUNCTION(BlueprintCallable)
	void ConvertToOresShop();

	UFUNCTION(BlueprintCallable)
	void ConvertToVegetableShop();

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

	UFUNCTION(BlueprintCallable)
	FString GetMessage(EMessageType MT);

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	int LandTypeNum;

private:
	UPROPERTY(EditAnywhere)
	int32 LandID;
};
