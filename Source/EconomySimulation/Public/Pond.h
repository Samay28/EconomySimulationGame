// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Land.h"
#include "Pond.generated.h"

/**
 *
 */
UCLASS()
class ECONOMYSIMULATION_API APond : public ALand
{
	GENERATED_BODY()

public:
	APond();

	// UPROPERTY(EditAnywhere, BlueprintReadOnly)
	// bool IsPurchased;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent *WaterMesh;

	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable)
	void TransferItems();

	UFUNCTION(BlueprintCallable)
	void SetupPond();

	void ProvideResourcesToStorage();

	int PondLevel;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	class UBusinessStorageComponent* StorageComponent;

protected:
	virtual void BeginPlay() override;
	int PondSetupCost;

private:

	FTimerHandle ItemsProvider;
};
