// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyPlayerCharacter.h"
#include "CarpenterShop.generated.h"

UCLASS()
class ECONOMYSIMULATION_API ACarpenterShop : public AActor
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;
	ACarpenterShop();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Shop")
    void SellItem(AMyPlayerCharacter* PlayerCharacter, FName ItemName, int32 Quantity, int32 Value);

	class AGameManager* GM;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ShopMesh;
};
