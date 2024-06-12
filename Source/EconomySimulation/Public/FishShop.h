// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyPlayerCharacter.h"
#include "FishShop.generated.h"

UCLASS()
class ECONOMYSIMULATION_API AFishShop : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFishShop();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Shop")
	void SellItem(AMyPlayerCharacter *PlayerCharacter);

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent *ShopMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	class AGameManager *GM;
};
