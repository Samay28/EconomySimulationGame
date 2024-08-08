// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Animal_AIController.generated.h"

/**
 * 
 */
UCLASS()
class ECONOMYSIMULATION_API AAnimal_AIController : public AAIController
{
	GENERATED_BODY()
public:
	explicit AAnimal_AIController(FObjectInitializer const& ObjectInitializer); //explicit to ensure that the initialization of objects is always clear and intentional.

protected:
	virtual void OnPossess(APawn* InPawn) override;
};
