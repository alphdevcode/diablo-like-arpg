// Copyright 2024 AlphDevCode. All Rights Reserved.

#include "DiabloLikeARPGPlayerController.h"

#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Actors/SpawnPoint.h"
#include "Blueprint/UserWidget.h"
#include "Characters/DiabloLikeARPGCharacter.h"
#include "Components/AbilitiesComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utils/ActorFunctionLibrary.h"

ADiabloLikeARPGPlayerController::ADiabloLikeARPGPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

void ADiabloLikeARPGPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	if (HudWidget)
	{
		HudWidget->RemoveFromParent();
	}

	if (bIsWinner)
	{
		// if (UUserWidget* WinScreenWidget = CreateWidget(this, WinScreenWidgetClass))
		// {
		// 	WinScreenWidget->AddToViewport();
		// }
	}
	else
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(DefaultMappingContext);
		}
		// if (UUserWidget* LoseScreenWidget = CreateWidget(this, LoseScreenWidgetClass))
		// {
		// 	LoseScreenWidget->AddToViewport();
		// }
	}

	FTimerHandle RestartTimerHandle;
	GetWorldTimerManager().SetTimer(RestartTimerHandle, this,
									&ADiabloLikeARPGPlayerController::RestartLevel, 2.f, false);
}

void ADiabloLikeARPGPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// if(bShouldLookAtMouseCursor) LookAtToMouseCursor();
}

void ADiabloLikeARPGPlayerController::RespawnPlayer()
{
	TArray<AActor*> SpawnPointArray;
	UGameplayStatics::GetAllActorsOfClass(this, ASpawnPoint::StaticClass(), SpawnPointArray);
	
	if(SpawnPointArray.IsValidIndex(0))
	{
		// ControlledCharacter->SetActorLocationAndRotation(SpawnPointArray[0]->GetActorLocation(),
		// 	SpawnPointArray[0]->GetActorRotation(), false, nullptr,
		// 	ETeleportType::ResetPhysics);
		// ControlledCharacter->HandleRespawn();
	}
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

	HudWidget = CreateWidget(this, HudWidgetClass);
	if (HudWidget)
	{
		HudWidget->AddToViewport();
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
		EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Started, this,
		                                   &ADiabloLikeARPGPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Triggered, this,
		                                   &ADiabloLikeARPGPlayerController::OnClickTriggered);
		EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Completed, this,
		                                   &ADiabloLikeARPGPlayerController::OnClickReleased);
		EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Canceled, this,
		                                   &ADiabloLikeARPGPlayerController::OnClickReleased);

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

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this,
										   &ADiabloLikeARPGPlayerController::OnMoveStarted);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this,
		                                   &ADiabloLikeARPGPlayerController::OnMoveTriggered);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this,
		                                   &ADiabloLikeARPGPlayerController::OnMoveReleased);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Canceled, this,
		                                   &ADiabloLikeARPGPlayerController::OnMoveReleased);

		EnhancedInputComponent->BindAction(SetRotateCameraAction, ETriggerEvent::Started, this,
											   &ADiabloLikeARPGPlayerController::OnSetRotateCameraStarted);
		EnhancedInputComponent->BindAction(SetRotateCameraAction, ETriggerEvent::Completed, this,
											   &ADiabloLikeARPGPlayerController::OnSetRotateCameraReleased);
		EnhancedInputComponent->BindAction(SetRotateCameraAction, ETriggerEvent::Canceled, this,
											   &ADiabloLikeARPGPlayerController::OnSetRotateCameraReleased);
		
		EnhancedInputComponent->BindAction(LookHorizontalAction, ETriggerEvent::Triggered, this,
		                                   &ADiabloLikeARPGPlayerController::OnLookHorizontalTriggered);

		SetupAbilitiesInput(EnhancedInputComponent);
	}
}


void ADiabloLikeARPGPlayerController::SetupAbilitiesInput(UEnhancedInputComponent* EnhancedInputComponent)
{
	
	EnhancedInputComponent->BindAction(ActivateAbility1Action, ETriggerEvent::Started, this,
										   &ADiabloLikeARPGPlayerController::OnAbilityActivated, 0);
	EnhancedInputComponent->BindAction(ActivateAbility2Action, ETriggerEvent::Started, this,
										   &ADiabloLikeARPGPlayerController::OnAbilityActivated, 1);
	EnhancedInputComponent->BindAction(ActivateAbility3Action, ETriggerEvent::Started, this,
										   &ADiabloLikeARPGPlayerController::OnAbilityActivated, 2);
	EnhancedInputComponent->BindAction(ActivateAbility4Action, ETriggerEvent::Started, this,
										   &ADiabloLikeARPGPlayerController::OnAbilityActivated, 3);

	// We're currently handling Left Click input from the interactable interface,
	// so it can trigger multiple actions based on the context. The action was left here,
	// but it's not bound in the mapping context.
	EnhancedInputComponent->BindAction(ActivateClickAbility1Action, ETriggerEvent::Started, this,
										   &ADiabloLikeARPGPlayerController::OnClickAbilityActivated, 0);
	EnhancedInputComponent->BindAction(ActivateClickAbility2Action, ETriggerEvent::Started, this,
										   &ADiabloLikeARPGPlayerController::OnClickAbilityActivated, 1);
}

void ADiabloLikeARPGPlayerController::OnAbilityActivated(const int AbilityIndex)
{
	ControlledCharacter->GetAbilitiesComponent()->ActivateAbility(AbilityIndex);
}

void ADiabloLikeARPGPlayerController::OnClickAbilityActivated(const int AbilityIndex)
{
	ControlledCharacter->GetAbilitiesComponent()->ActivateClickAbility(AbilityIndex);
}

void ADiabloLikeARPGPlayerController::OnRootedStarted()
{
	if (ControlledCharacter != nullptr)
	{
		ControlledCharacter->GetCharacterMovement()
		                   ->bUseControllerDesiredRotation = false;
		ControlledCharacter->GetCharacterMovement()
						   ->bOrientRotationToMovement = false;
	}
}

void ADiabloLikeARPGPlayerController::OnRootedTriggered()
{
	// UActorFunctionLibrary::LookAtDestination(ControlledCharacter, CachedDestination);
	FHitResult HitResult;
	if(GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, HitResult))
	{
		RotatorToMouseCursor = UActorFunctionLibrary::LookAtDestination(ControlledCharacter, HitResult.Location);
	}
}

void ADiabloLikeARPGPlayerController::OnRootedReleased()
{
	if (ControlledCharacter != nullptr)
	{
		ControlledCharacter->GetCharacterMovement()
						   ->bUseControllerDesiredRotation = true;
		ControlledCharacter->GetCharacterMovement()
						   ->bOrientRotationToMovement = true;
	}
}

void ADiabloLikeARPGPlayerController::OnMoveStarted()
{
	StopMovement();
	bIsManualMoving = true;
}

void ADiabloLikeARPGPlayerController::OnMoveTriggered(const FInputActionValue& ActionValue)
{
	if(!bIsManualMoving) return;
	
	if (ControlledCharacter != nullptr && ActionValue.GetValueType() == EInputActionValueType::Axis2D)
	{
		const FVector RightDirection = UKismetMathLibrary::GetRightVector(
			FRotator(0.f, GetControlRotation().Yaw, GetControlRotation().Roll));
		ControlledCharacter->AddMovementInput(RightDirection,
			ActionValue.Get<FInputActionValue::Axis2D>().X);

		const FVector ForwardDirection = UKismetMathLibrary::GetForwardVector(
			FRotator(0.f, GetControlRotation().Yaw, 0.f));
		ControlledCharacter->AddMovementInput(ForwardDirection,
			ActionValue.Get<FInputActionValue::Axis2D>().Y);
	}
}

void ADiabloLikeARPGPlayerController::OnMoveReleased()
{
	bIsManualMoving = false;
}

void ADiabloLikeARPGPlayerController::OnLookHorizontalTriggered(const FInputActionValue& ActionValue)
{
	if(bCanRotateCamera && ActionValue.GetValueType() == EInputActionValueType::Axis1D)
	{
		ControlledCharacter->AddControllerYawInput(ActionValue.Get<FInputActionValue::Axis1D>());
	}
}

void ADiabloLikeARPGPlayerController::OnSetRotateCameraStarted()
{
	bCanRotateCamera = true;
}

void ADiabloLikeARPGPlayerController::OnSetRotateCameraReleased()
{
	bCanRotateCamera = false;
}

void ADiabloLikeARPGPlayerController::LookAtToMouseCursor()
{
	FHitResult HitResult;
	if(GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, HitResult))
	{
		UActorFunctionLibrary::LookAtDestination(ControlledCharacter, HitResult.Location);
	}
}

void ADiabloLikeARPGPlayerController::OnInputStarted()
{
	StopMovement();
}

void ADiabloLikeARPGPlayerController::MoveTo(const FVector& Destination) const
{
	FVector WorldDirection = (Destination - ControlledCharacter->GetActorLocation()).GetSafeNormal();
	ControlledCharacter->AddMovementInput(WorldDirection, 1.0, false);
}

void ADiabloLikeARPGPlayerController::OnClickTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();

	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bHitDamageableActor = true;
	// Check if it hit any damageable actor
	bool bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1, true, Hit);

	// if(GEngine && bHitSuccessful)
	//  		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange,
	// 		"Hit Damageable");

	// If it didn't hit any damageable actor, check if it hit any surface
	if (!bHitSuccessful)
	{
		bHitDamageableActor = false;
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
		// if(GEngine && bHitSuccessful)
		// 	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange,
		//    "Hit Visibility");
	}

	// If we hit a surface or a damageable actor, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}

	if (bHitDamageableActor)
	{
		// Invalidate current manual moving (if active), so player can move to hit damageable actor
		bIsManualMoving = false;

		if (FVector::Dist(CachedDestination, ControlledCharacter->GetActorLocation())
			<= ControlledCharacter->GetInteractionRange())
		{
			ControlledCharacter->ActivatePrimaryAttackAbility();
		}
		else
		{
			MoveTo(CachedDestination);
		}
	}
	else
	{
		if(!bIsManualMoving)
		{
			MoveTo(CachedDestination);
		}
	}
}

void ADiabloLikeARPGPlayerController::ContinuouslyMoveToLocation(const FVector& Location)
{
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, Location);
}

void ADiabloLikeARPGPlayerController::OnClickReleased()
{
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		if(bIsManualMoving || bHitDamageableActor)
		{
			return;
		}
		// We move there and spawn some particles
		ContinuouslyMoveToLocation(CachedDestination);
		if(!bHitDamageableActor)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator,
														   FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
		}
	}

	FollowTime = 0.f;
}

// Triggered every frame when the input is held down
void ADiabloLikeARPGPlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnClickTriggered();
}

void ADiabloLikeARPGPlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnClickReleased();
}
