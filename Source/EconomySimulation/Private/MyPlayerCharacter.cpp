// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TimelineComponent.h"
#include "GameManager.h"
#include "DrawDebugHelpers.h"
#include <Engine/World.h>

// Sets default values
AMyPlayerCharacter::AMyPlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    bShouldMoveForward = false;
    bUseControllerRotationYaw = false;
    Coins = 100;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComponent->SetupAttachment(SpringArm);

    PlayerInventoryComponent = CreateDefaultSubobject<UPlayerInventoryComponent>(TEXT("PlayerInventory"));
    StartLocationForJungle = FVector(-200, 6680, 300);
    JungleStartLocation = FVector(64072.112698f, 129536.133462f, 299.611306f);

    IsReadyToAutoMove = false;
}

void AMyPlayerCharacter::TeleportPlayer()
{
    if (!GM->BlockPlayerMovement)
    {
        SetActorLocation(JungleStartLocation);
        if (SpringArm)
        {
            SpringArm->TargetArmLength = 211.327728f;
            SpringArm->SetRelativeRotation(FRotator(00, 90, 0.f));
            SpringArm->SocketOffset = FVector(0.0f, 0.0f, 16.396455f);
            SpringArm->TargetOffset = FVector(0.0f, 0.0f, 178.514561f);
            SpringArm->bInheritYaw = true;
            SpringArm->bUsePawnControlRotation = false;
        }
    }
}

void AMyPlayerCharacter::SendBackPlayer()
{
    if (!GM->BlockPlayerMovement)
    {
        SetActorLocation(StartLocationForJungle);
        if (SpringArm)
        {
            SpringArm->TargetArmLength = 1390.f;
            SpringArm->SetRelativeRotation(FRotator(0, -90, 00.f));
            SpringArm->SocketOffset = FVector(0.0f, 0.0f, 365.f);
            SpringArm->TargetOffset = FVector(0.0f, 0.0f, 450.f);
            SpringArm->bInheritYaw = false;
            SpringArm->bUsePawnControlRotation = true;
        }
    }
}

void AMyPlayerCharacter::FaceNearestActorWithTag(FString Tag)
{
    AActor *NearestActor = FindNearestActorWithTag(Tag);
    if (NearestActor)
    {
        FVector Direction = NearestActor->GetActorLocation() - GetActorLocation();
        Direction.Z = 0;
        FRotator NewRotation = Direction.Rotation();
        NewRotation.Pitch = 0; // Prevent rotation on the Pitch axis
        NewRotation.Roll = 0;  // roll
        SetActorRotation(NewRotation);
        EndLocation = NearestActor->GetActorLocation();
        IsReadyToAutoMove = true;
    }
}

AActor *AMyPlayerCharacter::FindNearestActorWithTag(FString Tag)
{

    float MinDistance = FLT_MAX;
    AActor *NearestActor = nullptr;

    TArray<AActor *> TaggedActors;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(*Tag), TaggedActors);

    for (AActor *Actor : TaggedActors)
    {
        float Distance = FVector::Dist(GetActorLocation(), Actor->GetActorLocation());
        if (Distance < MinDistance)
        {
            MinDistance = Distance;
            NearestActor = Actor;
        }
    }
    return NearestActor;
}

void AMyPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
    CurrentRotation = GetActorRotation();

    AActor *FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass());
    GM = Cast<AGameManager>(FoundActor);
}

void AMyPlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    bool hasMoved = false;

    FVector Start = GetActorLocation();
    FVector ForwardVector = GetActorForwardVector();
    FVector End = ((ForwardVector * 100) + Start);
    FHitResult Hit;

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);

    // Perform the line trace
    bool bHasHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, CollisionParams);

    if (bHasHit)
    {
        StopMovement();
    }
    else if (bShouldMoveForward && !GM->BlockPlayerMovement)
    {
        AddMovementInput(GetActorForwardVector(), 1.0f);
    }

    // Normalize the movement direction
    if (hasMoved)
    {
        MovementDirection.Normalize();
        AddMovementInput(MovementDirection, 1.0f);
    }
    if (IsReadyToAutoMove)
    {
        FVector Direction = (EndLocation - GetActorLocation()).GetSafeNormal();
        AddMovementInput(Direction, 1.0f);

        FVector CurrentLocation = GetActorLocation();
        if (FMath::IsNearlyEqual(CurrentLocation.X, EndLocation.X, 5.0f) &&
            FMath::IsNearlyEqual(CurrentLocation.Y, EndLocation.Y, 5.0f)) // Check if X and Y are close enough to the target
        {
            IsReadyToAutoMove = false;
            UE_LOG(LogTemp, Warning, TEXT("Called"));
            StopMovement(); // Stop movement once close to the target
        }
    }
}

void AMyPlayerCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMyPlayerCharacter::MoveForward()
{
    bShouldMoveForward = true;
}

void AMyPlayerCharacter::MoveBackward()
{
    FRotator NewRot = FRotator(CurrentRotation.Pitch, CurrentRotation.Yaw + 180, CurrentRotation.Roll);
    bShouldMoveForward = false;
    SetActorRotation(NewRot);
    CurrentRotation = GetActorRotation();
}

void AMyPlayerCharacter::MoveLeft()
{

    FRotator NewRot = FRotator(CurrentRotation.Pitch, CurrentRotation.Yaw - 90, CurrentRotation.Roll);
    UE_LOG(LogTemp, Warning, TEXT("HA"));
    SetActorRotation(NewRot);
    CurrentRotation = GetActorRotation();
}

void AMyPlayerCharacter::MoveRight()
{

    FRotator NewRot = FRotator(CurrentRotation.Pitch, CurrentRotation.Yaw + 90, CurrentRotation.Roll);

    SetActorRotation(NewRot);
    CurrentRotation = GetActorRotation();
}

void AMyPlayerCharacter::StopMovement()
{
    GetCharacterMovement()->StopMovementImmediately();
    bShouldMoveForward = false;
    IsReadyToAutoMove = false;
}
