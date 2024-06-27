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
	MT_InsufficientFunds UMETA(DisplayName = "Insufficient Coins"),
	MT_SuccessfullPurchase UMETA(DisplayName = "Purchase Done"),
	MT_InsufficientIslandValue UMETA(DisplayName = "Increase your Island Value to : ")
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
	FString PurchaseLand();
	static bool IsStoragePresent;

	UFUNCTION(BlueprintCallable)
	FString ConvertToFarm();
	static bool IsVegetableShopPresent;

	UFUNCTION(BlueprintCallable)
	FString ConvertToHouse();
	static bool IsATMPresent;

	UFUNCTION(BlueprintCallable)
	FString ConvertToPond();
	static bool IsFishShopPresent;

	UFUNCTION(BlueprintCallable)
	FString ConvertToMiningLand();
	static bool IsOreShopPresent;

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

	UPROPERTY(BlueprintReadOnly)
	bool AreItemsReady;

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	int LandTypeNum;

private:
	UPROPERTY(EditAnywhere)
	int32 LandID;
};
