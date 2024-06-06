// Fill out your copyright notice in the Description page of Project Settings.


#include "CarpenterShop.h"
#include "Kismet/GameplayStatics.h"
#include "GameManager.h"

// Sets default values
ACarpenterShop::ACarpenterShop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACarpenterShop::BeginPlay()
{
	Super::BeginPlay();
	AActor *FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass());
	GM = Cast<AGameManager>(FoundActor);
}

// Called every frame
void ACarpenterShop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ACarpenterShop::SellItem(AMyPlayerCharacter *PlayerCharacter, FName ItemName, int32 Quantity, int32 Value)
{
	if (PlayerCharacter && PlayerCharacter->PlayerInventoryComponent)
    {
        if (PlayerCharacter->PlayerInventoryComponent->HasItem(ItemName, Quantity))
        {
            PlayerCharacter->PlayerInventoryComponent->RemoveItem(ItemName, Quantity);
            GM->coins += Value*Quantity;
            UE_LOG(LogTemp, Warning, TEXT("Sold %d %s"), Quantity, *ItemName.ToString());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Not enough items to sell"));
        }
    }
}
