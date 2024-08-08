// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Land.h"
#include "MiningLand.generated.h"


UCLASS()
class ECONOMYSIMULATION_API AMiningLand : public ALand
{
	GENERATED_BODY()
public:
	AMiningLand();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsPurchased;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent *MineMesh;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void TransferItems();

	UFUNCTION(BlueprintCallable)
	void SetupMiningLand();

	void ProvideResourcesToStorage();

	int MiningLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UBusinessStorageComponent *StorageComponent;

protected:
	virtual void BeginPlay() override;
	int MiningLandSetupCost;

private:
	

	FTimerHandle ItemsProvider;
};
