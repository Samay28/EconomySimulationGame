#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LandSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FLandSaveData
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, Category = "SaveData")
    int32 LandID;

    UPROPERTY(VisibleAnywhere, Category = "SaveData")
    int LandTypeNum;

    UPROPERTY(VisibleAnywhere, Category = "SaveData")
    FVector Location;

    UPROPERTY(VisibleAnywhere, Category = "SaveData")
    FRotator Rotation;
};

UCLASS()
class ECONOMYSIMULATION_API ULandSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, Category = "SaveData")
    TArray<FLandSaveData> LandDataArray;
};
