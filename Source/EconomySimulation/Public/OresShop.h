
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyPlayerCharacter.h"
#include "OresShop.generated.h"

UCLASS()
class ECONOMYSIMULATION_API AOresShop : public AActor
{
	GENERATED_BODY()

public:
	AOresShop();

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
