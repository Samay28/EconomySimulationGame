// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FindRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Animal_AIController.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation(FObjectInitializer const &ObjectInitializer)
{
    NodeName = "Find Random Location";
    SearchRadius = 1000.f;
}
EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    UE_LOG(LogTemp, Warning, TEXT("done2 "));
    if (auto *const cont = Cast<AAnimal_AIController>(OwnerComp.GetAIOwner()))
    {
        if (auto *const Animal = cont->GetPawn())
        {
            auto const Origin = Animal->GetActorLocation();

            if (auto *const NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
            {
                FNavLocation Loc;
                if (NavSys->GetRandomPointInNavigableRadius(Origin, SearchRadius, Loc))
                {
                    OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Loc.Location);
                }
                UE_LOG(LogTemp, Warning, TEXT("Total coins yea"));
                FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
                return EBTNodeResult::Succeeded;
            }
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("Total coins "));
    return EBTNodeResult::Failed;
}