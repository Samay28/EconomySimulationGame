// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyPlayerCharacter.h"
#include "VegetableShop.generated.h"

UCLASS()
class ECONOMYSIMULATION_API AVegetableShop : public AActor
{
	GENERATED_BODY()

public:
	AVegetableShop();

	UFUNCTION(BlueprintCallable, Category = "Shop")
	void SellItem(AMyPlayerCharacter *PlayerCharacter);

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent *ShopMesh;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	class AGameManager *GM;
};
