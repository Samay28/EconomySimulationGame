#include "BusinessStorageComponent.h"
#include "StorageSaveClass.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UBusinessStorageComponent::UBusinessStorageComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UBusinessStorageComponent::SaveStorage()
{
    UStorageSaveClass *SaveGameInstance;
    if (UGameplayStatics::DoesSaveGameExist(TEXT("StorageSlot"), 0))
    {
        SaveGameInstance = Cast<UStorageSaveClass>(UGameplayStatics::LoadGameFromSlot(TEXT("StorageSlot"), 0));
        if (!SaveGameInstance)
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to load save game instance from slot: StorageSlot"));
            return;
        }
    }
    else
    {
        SaveGameInstance = Cast<UStorageSaveClass>(UGameplayStatics::CreateSaveGameObject(UStorageSaveClass::StaticClass()));
        if (!SaveGameInstance)
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to create save game instance for Storage"));
            return;
        }
    }

    // Merge current storage with saved storage
    for (const FStorageItem &CurrentItem : Storage)
    {
        bool bItemFound = false;
        for (FStorageItemData &SavedItem : SaveGameInstance->SavedStorage)
        {
            if (SavedItem.ItemName == CurrentItem.ItemName)
            {
                SavedItem.Quantity += CurrentItem.Quantity;
                SavedItem.Value = CurrentItem.Value;
                bItemFound = true;
                break;
            }
        }
        if (!bItemFound)
        {
            SaveGameInstance->SavedStorage.Add(FStorageItemData(CurrentItem.ItemName, CurrentItem.Quantity, CurrentItem.Value));
        }
    }

    // Merge current item values with saved item values
    for (const auto &CurrentItemValue : ItemValues)
    {
        SaveGameInstance->SavedItemValues.FindOrAdd(CurrentItemValue.Key) = CurrentItemValue.Value;
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
        UE_LOG(LogTemp, Warning, TEXT("Loaded game instance, number of saved items: %d"), LoadGameInstance->SavedStorage.Num());

        for (const FStorageItemData &SavedItem : LoadGameInstance->SavedStorage)
        {
            FStorageItem *FoundItem = Storage.FindByPredicate([&](const FStorageItem &Item)
                                                              { return Item.ItemName == SavedItem.ItemName; });
            if (FoundItem)
            {
                FoundItem->Quantity += SavedItem.Quantity;
                FoundItem->Value = SavedItem.Value;
            }
            else
            {
                Storage.Add(FStorageItem(SavedItem.ItemName, SavedItem.Quantity, SavedItem.Value));
            }
            UE_LOG(LogTemp, Warning, TEXT("Loaded item - Name: %s, Quantity: %d, Value: %d"), *SavedItem.ItemName.ToString(), SavedItem.Quantity, SavedItem.Value);
        }

        UE_LOG(LogTemp, Warning, TEXT("Loaded game instance, number of saved item values: %d"), LoadGameInstance->SavedItemValues.Num());

        for (const auto &SavedItemValue : LoadGameInstance->SavedItemValues)
        {
            ItemValues.FindOrAdd(SavedItemValue.Key) = SavedItemValue.Value;
            UE_LOG(LogTemp, Warning, TEXT("Loaded item value - Name: %s, Value: %d"), *SavedItemValue.Key.ToString(), SavedItemValue.Value);
        }

        UE_LOG(LogTemp, Warning, TEXT("Storage loaded successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load storage"));
    }
}

// Called when the game starts
void UBusinessStorageComponent::BeginPlay()
{
    Super::BeginPlay();
    LoadStorage();
}

// Called every frame
void UBusinessStorageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}

void UBusinessStorageComponent::AddItem(FName ItemName, int32 Quantity, int32 Value)
{
    if (Quantity <= 0)
        return;

    FStorageItem *FoundItem = Storage.FindByPredicate([&](const FStorageItem &Item)
                                                      { return Item.ItemName == ItemName; });

    if (FoundItem)
    {
        FoundItem->Quantity += Quantity;
        FoundItem->Value = Value; // Ensure the value is updated if item already exists
    }
    else
    {
        Storage.Add(FStorageItem(ItemName, Quantity, Value));
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
    for (int32 i = 0; i < Storage.Num(); ++i)
    {
        if (Storage[i].ItemName == ItemName)
        {
            Storage[i].Quantity -= Quantity;
            if (Storage[i].Quantity <= 0)
            {
                Storage.RemoveAt(i);
            }
            return;
        }
    }
    SaveStorage();
}

bool UBusinessStorageComponent::HasItem(FName ItemName, int32 Quantity) const
{
    for (const FStorageItem &Item : Storage)
    {
        if (Item.ItemName == ItemName && Item.Quantity >= Quantity)
        {
            return true;
        }
    }
    return false;
}

int32 UBusinessStorageComponent::GetItemQuantity(FName ItemName) const
{
    for (const FStorageItem &Item : Storage)
    {
        if (Item.ItemName == ItemName)
        {
            return Item.Quantity;
        }
    }
    return 0;
}

TArray<FStorageItem> UBusinessStorageComponent::GetItems() const
{
    return Storage;
}

void UBusinessStorageComponent::ClearItems()
{
    Storage.Empty();
}
