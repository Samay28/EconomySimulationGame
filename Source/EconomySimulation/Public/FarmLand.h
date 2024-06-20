// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Land.h"
#include "SaveGameInterface.h"
#include "BusinessStorageComponent.h"
#include "FarmLand.generated.h"


UCLASS()
class ECONOMYSIMULATION_API AFarmLand : public ALand
{
	GENERATED_BODY()

public:
	AFarmLand();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent *Carrots;

	UFUNCTION(BlueprintCallable)
	void RentLandForFarming();

	UFUNCTION(BlueprintCallable)
	void TransferItems();

	void HarvestCropsToStorage();

	 void SaveGame() ;	
	 void LoadGame() ;

	class AMyPlayerCharacter* Player;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Storage")
	UBusinessStorageComponent* StorageComponent;
protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	int FarmSetupCost;
	int count;

private:

	UPROPERTY(EditAnywhere, Category = "Farm")
    int32 FarmID;

	FTimerHandle ResourceGeneratingHandle;
	
};
