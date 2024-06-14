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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsRented;
	int LandCost;

	void DeductRent();

	class AGameManager *GM;
	class AMyPlayerCharacter* Player;

	UFUNCTION(BlueprintCallable)
	void PurchaseLand();

	UFUNCTION(BlueprintCallable)
	void ConvertToFarm();

	UFUNCTION(BlueprintCallable)
	void ConvertToHouse();

	UFUNCTION(BlueprintCallable)
	void ConvertToPond();

	UFUNCTION(BlueprintCallable)
	void ConvertToCarpenterShop();

	UFUNCTION(BlueprintCallable)
	void ConvertToFishShop();

	UFUNCTION(BlueprintCallable)
	void ConvertToOresShop();

	UFUNCTION(BlueprintCallable)
	void KeepSimpleLand();

	UPROPERTY(EditDefaultsOnly, Category = "Conversion")
	TSubclassOf<class AHouseLand> HouseLandBlueprint;

	UPROPERTY(EditDefaultsOnly, Category = "Conversion")
	TSubclassOf<class AFarmLand> FarmLandBlueprint;

	UPROPERTY(EditDefaultsOnly, Category = "Conversion")
	TSubclassOf<class APond> PondBlueprint;

	UPROPERTY(EditDefaultsOnly, Category = "Conversion")
	TSubclassOf<class ACarpenterShop> CarpenterShopBlueprint;

	UPROPERTY(EditAnywhere, Category = "Conversion")
	TSubclassOf<class AFishShop> FishShopBlueprint;

	UPROPERTY(EditAnywhere, Category = "Conversion")
	TSubclassOf<class AOresShop> OresShopBlueprint;

	UPROPERTY()
	TArray<AActor *> CreatedActors;

	UFUNCTION(BlueprintCallable)
	void SaveGame();

	UFUNCTION(BlueprintCallable)
	void LoadGame();

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	int LandTypeNum; // 1 for farm, 2 for house, 0 for plain land

private:
	UPROPERTY(EditAnywhere)
	int32 LandID;
};
