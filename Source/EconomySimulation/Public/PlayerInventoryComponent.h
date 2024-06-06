// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity;

	FInventoryItem()   //This is the default constructor. It initializes an FInventoryItem instance with default values.
		: ItemName(NAME_None), Quantity(0)
	{
	}

	FInventoryItem(FName NewItemName, int32 NewQuantity) //This constructor allows the creation of an FInventoryItem instance with specific values for ItemName and Quantity.
		: ItemName(NewItemName), Quantity(NewQuantity)
	{
	}

	bool operator==(const FInventoryItem& Other) const
    {
        return ItemName == Other.ItemName && Quantity == Other.Quantity;
    }
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ECONOMYSIMULATION_API UPlayerInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	TArray<FInventoryItem> Inventory;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItem(FName ItemName, int32 Quantity);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItem(FName ItemName, int32 Quantity);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool HasItem(FName ItemName, int32 Quantity) const;
};
