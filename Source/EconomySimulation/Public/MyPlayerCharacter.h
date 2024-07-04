// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerInventoryComponent.h"
#include "MyPlayerCharacter.generated.h"

UCLASS()
class ECONOMYSIMULATION_API AMyPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyPlayerCharacter();

	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Controls")
	void MoveForward();

	UFUNCTION(BlueprintCallable, Category = "Controls")
	void MoveBackward();

	UFUNCTION(BlueprintCallable, Category = "Controls")
	void MoveLeft();

	UFUNCTION(BlueprintCallable, Category = "Controls")
	void MoveRight();

	UFUNCTION(BlueprintCallable, Category = "Controls")
	void StopMovement();

	bool bShouldMoveForward;
	FRotator CurrentRotation;
	FVector StartLocationForJungle;
	FVector JungleStartLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Economy")
	int Coins;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent *SpringArm;

	UPROPERTY(EditAnywhere)
	class UCameraComponent *CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	UPlayerInventoryComponent *PlayerInventoryComponent;

	// jungle
	UFUNCTION(BlueprintCallable)
	void TeleportPlayer();

	UFUNCTION(BlueprintCallable)
	void SendBackPlayer();

	// new
	UFUNCTION(BlueprintCallable)
	void FaceNearestActorWithTag(FString Tag);

protected:
	virtual void BeginPlay() override;

	FVector MovementDirection;

	class AGameManager *GM;

	AActor *FindNearestActorWithTag(FString Tag);
	bool IsReadyToAutoMove;

	FVector StartLocation;
	FVector EndLocation;
};