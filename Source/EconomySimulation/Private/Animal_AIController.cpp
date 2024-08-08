// Fill out your copyright notice in the Description page of Project Settings.


#include "Animal_AIController.h"
#include "AnimalCharacter.h"


AAnimal_AIController::AAnimal_AIController(FObjectInitializer const& ObjectInitializer)
{

}

void AAnimal_AIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    if(AAnimalCharacter* const animal = Cast<AAnimalCharacter>(InPawn))
    {
        if(UBehaviorTree* const tree = animal->GetBehaviorTree())
        {
            UBlackboardComponent* b;
            UseBlackboard(tree->BlackboardAsset, b);
            Blackboard = b;
            RunBehaviorTree(tree);
            UE_LOG(LogTemp, Warning, TEXT("done1"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("?"));
        }
    }
}

