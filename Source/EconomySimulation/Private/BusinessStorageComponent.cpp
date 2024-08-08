#include "BusinessStorageComponent.h"
#include "StorageSaveClass.h"
#include "Kismet/GameplayStatics.h"

TMap<FName, int32> UBusinessStorageComponent::GlobalItemQuantities;
UBusinessStorageComponent::UBusinessStorageComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UBusinessStorageComponent::BeginPlay()
{
    Super::BeginPlay();
    LoadStorage();
}

// Called every frame
void UBusinessStorageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UBusinessStorageComponent::SaveStorage()
{
    UStorageSaveClass *SaveGameInstance = Cast<UStorageSaveClass>(UGameplayStatics::CreateSaveGameObject(UStorageSaveClass::StaticClass()));

    for (const auto &ItemQuantity : GlobalItemQuantities)
    {
        SaveGameInstance->SavedStorage.Add(FStorageItemData(ItemQuantity.Key, ItemQuantity.Value, ItemValues.Contains(ItemQuantity.Key) ? ItemValues[ItemQuantity.Key] : 0));
        UE_LOG(LogTemp, Warning, TEXT("Saving item - Name: %s, Quantity: %d"), *ItemQuantity.Key.ToString(), ItemQuantity.Value);
    }

    for (const auto &ItemValue : ItemValues)
    {
        SaveGameInstance->SavedItemValues.Add(ItemValue.Key, ItemValue.Value);
    }

    if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("StorageSlot"), 0))
    {
        UE_LOG(LogTemp, Warning, TEXT("Storage saved successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to save storage"));
    }
}

void UBusinessStorageComponent::LoadStorage()
{
    UStorageSaveClass *LoadGameInstance = Cast<UStorageSaveClass>(UGameplayStatics::LoadGameFromSlot(TEXT("StorageSlot"), 0));
    if (LoadGameInstance)
    {
        GlobalItemQuantities.Empty();
        ItemValues.Empty();

        UE_LOG(LogTemp, Warning, TEXT("Loaded game instance, number of saved items: %d"), LoadGameInstance->SavedStorage.Num());

        for (const FStorageItemData &ItemData : LoadGameInstance->SavedStorage)
        {
            GlobalItemQuantities.Add(ItemData.ItemName, ItemData.Quantity);
            ItemValues.Add(ItemData.ItemName, ItemData.Value);
            UE_LOG(LogTemp, Warning, TEXT("Loaded item - Name: %s, Quantity: %d, Value: %d"), *ItemData.ItemName.ToString(), ItemData.Quantity, ItemData.Value);
        }

        for (const auto &ItemValue : LoadGameInstance->SavedItemValues)
        {
            ItemValues.Add(ItemValue.Key, ItemValue.Value);
            UE_LOG(LogTemp, Warning, TEXT("Loaded item value - Name: %s, Value: %d"), *ItemValue.Key.ToString(), ItemValue.Value);
        }

        UE_LOG(LogTemp, Warning, TEXT("Storage loaded successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load storage"));
    }
}


// Called when the game starts


void UBusinessStorageComponent::AddItem(FName ItemName, int32 Quantity, int32 Value)
{
    if (Quantity <= 0)
        return;

    if (GlobalItemQuantities.Contains(ItemName))
    {
        GlobalItemQuantities[ItemName] += Quantity;
    }
    else
    {
        GlobalItemQuantities.Add(ItemName, Quantity);
    }

    if (ItemValues.Contains(ItemName))
    {
        ItemValues[ItemName] = Value;
    }
    else
    {
        ItemValues.Add(ItemName, Value);
    }

    SaveStorage();
}

void UBusinessStorageComponent::RemoveItem(FName ItemName, int32 Quantity)
{
    if (GlobalItemQuantities.Contains(ItemName))
    {
        GlobalItemQuantities[ItemName] -= Quantity;
        if (GlobalItemQuantities[ItemName] <= 0)
        {
            GlobalItemQuantities.Remove(ItemName);
        }

        SaveStorage();
    }
}

bool UBusinessStorageComponent::HasItem(FName ItemName, int32 Quantity) const
{
    const int32* FoundQuantity = GlobalItemQuantities.Find(ItemName);
    return FoundQuantity && *FoundQuantity >= Quantity;
}

int32 UBusinessStorageComponent::GetItemQuantity(FName ItemName) const
{
    const int32* FoundQuantity = GlobalItemQuantities.Find(ItemName);
    return FoundQuantity ? *FoundQuantity : 0;
}

TArray<FStorageItem> UBusinessStorageComponent::GetItems() const
{
    TArray<FStorageItem> Items;
    for (const auto &ItemQuantity : GlobalItemQuantities)
    {
        int32 Value = ItemValues.Contains(ItemQuantity.Key) ? ItemValues[ItemQuantity.Key] : 0;
        Items.Add(FStorageItem(ItemQuantity.Key, ItemQuantity.Value, Value));
    }
    return Items;
}

void UBusinessStorageComponent::ClearItems()
{
    Storage.Empty();
    UE_LOG(LogTemp, Error, TEXT("storage cleared"));
    SaveStorage();
}
