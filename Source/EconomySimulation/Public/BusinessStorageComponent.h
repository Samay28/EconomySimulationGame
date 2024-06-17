// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BusinessStorageComponent.generated.h"

USTRUCT(BlueprintType)
struct FStorageItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Value;

	FStorageItem() // This is the default constructor. It initializes an FStorageItem instance with default values.
		: ItemName(NAME_None), Quantity(0) , Value(0)
	{
	}

	FStorageItem(FName NewItemName, int32 NewQuantity, int32 NewValue) // This constructor allows the creation of an FStorageItem instance with specific values for ItemName and Quantity.
		: ItemName(NewItemName), Quantity(NewQuantity), Value(NewValue)
	{
	}

	bool operator==(const FStorageItem &Other) const
	{
		return ItemName == Other.ItemName && Quantity == Other.Quantity && Value == Other.Value;
	}
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ECONOMYSIMULATION_API UBusinessStorageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBusinessStorageComponent();
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SaveStorage();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void LoadStorage();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TArray<FStorageItem> Storage;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItem(FName ItemName, int32 Quantity, int32 Value);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItem(FName ItemName, int32 Quantity);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool HasItem(FName ItemName, int32 Quantity) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetItemQuantity(FName ItemName) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TMap<FName, int32> ItemValues;	

	TArray<FStorageItem> GetItems() const;	
	void ClearItems();
};
