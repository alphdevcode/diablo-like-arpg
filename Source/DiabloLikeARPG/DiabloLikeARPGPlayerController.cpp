// Copyright Epic Games, Inc. All Rights Reserved.

#include "DiabloLikeARPGPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "DiabloLikeARPGCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

ADiabloLikeARPGPlayerController::ADiabloLikeARPGPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

void ADiabloLikeARPGPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// Cache ControlledCharacter
	ControlledCharacter = Cast<ADiabloLikeARPGCharacter>(GetCharacter());
	if (ControlledCharacter == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ADiabloLikeARPGPlayerController::BeginPlay: ControlledCharacter is nullptr"));
	}
}

void ADiabloLikeARPGPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this,
		                                   &ADiabloLikeARPGPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this,
		                                   &ADiabloLikeARPGPlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this,
		                                   &ADiabloLikeARPGPlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this,
		                                   &ADiabloLikeARPGPlayerController::OnSetDestinationReleased);

		// Setup touch input events
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this,
		                                   &ADiabloLikeARPGPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this,
		                                   &ADiabloLikeARPGPlayerController::OnTouchTriggered);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this,
		                                   &ADiabloLikeARPGPlayerController::OnTouchReleased);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this,
		                                   &ADiabloLikeARPGPlayerController::OnTouchReleased);

		EnhancedInputComponent->BindAction(SetRootedAction, ETriggerEvent::Started, this,
		                                   &ADiabloLikeARPGPlayerController::OnRootedStarted);
		EnhancedInputComponent->BindAction(SetRootedAction, ETriggerEvent::Triggered, this,
		                                   &ADiabloLikeARPGPlayerController::OnRootedTriggered);
		EnhancedInputComponent->BindAction(SetRootedAction, ETriggerEvent::Completed, this,
		                                   &ADiabloLikeARPGPlayerController::OnRootedReleased);
		EnhancedInputComponent->BindAction(SetRootedAction, ETriggerEvent::Canceled, this,
		                                   &ADiabloLikeARPGPlayerController::OnRootedReleased);
	}
}

void ADiabloLikeARPGPlayerController::OnRootedStarted()
{
	if (ControlledCharacter != nullptr)
	{
		ControlledCharacter->GetCharacterMovement()
		                   ->SetMovementMode(EMovementMode::MOVE_None);
	}
}

void ADiabloLikeARPGPlayerController::OnRootedTriggered()
{
	LookAtDestination();
}

void ADiabloLikeARPGPlayerController::OnRootedReleased()
{
	if (ControlledCharacter != nullptr)
	{
		ControlledCharacter->GetCharacterMovement()
		                   ->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}

void ADiabloLikeARPGPlayerController::OnInputStarted()
{
	StopMovement();
}

// Triggered every frame when the input is held down
void ADiabloLikeARPGPlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();

	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}

	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void ADiabloLikeARPGPlayerController::OnSetDestinationReleased()
{
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator,
		                                               FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}

// Triggered every frame when the input is held down
void ADiabloLikeARPGPlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void ADiabloLikeARPGPlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}

void ADiabloLikeARPGPlayerController::LookAtDestination()
{
	if (ControlledCharacter == nullptr)
	{
		UE_LOG(LogTemp, Error,
		       TEXT("ADiabloLikeARPGPlayerController::LookAtDestination: ControlledCharacter is nullptr"));
		return;
	}

	const FRotator LookAtRotator = UKismetMathLibrary::FindLookAtRotation(
		ControlledCharacter->GetActorLocation(), CachedDestination);

	// if(GEngine)
	// 	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
	// 		LookAtRotator.ToString());	

	const FRotator InterpRotator = UKismetMathLibrary::RInterpTo(
		ControlledCharacter->GetActorRotation(),
		FRotator(0.f, LookAtRotator.Yaw, 0.f),
		GetWorld()->GetDeltaSeconds(),
		10.f);

	ControlledCharacter->SetActorRotation(InterpRotator);
}
