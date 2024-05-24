// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "DiabloLikeARPGPlayerController.generated.h"

class UNiagaraSystem;
/** Forward declaration to improve compiling times */
class UInputAction;

UCLASS()
class ADiabloLikeARPGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ADiabloLikeARPGPlayerController();

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Click Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationClickAction;

	/** Touch Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationTouchAction;

	/** Rooted Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetRootedAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* LookHorizontalAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetRotateCameraAction;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	bool bHitDamageableActor;

	UPROPERTY()
	class ADiabloLikeARPGCharacter* ControlledCharacter;

	virtual void SetupInputComponent() override;

	// To add mapping context
	virtual void BeginPlay();

	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void MoveTo(const FVector& Destination) const;
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();
	void OnTouchTriggered();
	void OnTouchReleased();

	/** Input handlers for SetRooted action. */
	void OnRootedStarted();
	void OnRootedTriggered();
	void OnRootedReleased();

	void OnMoveStarted();
	void OnMoveTriggered(const FInputActionValue& ActionValue);
	void OnMoveReleased();
	
	void OnLookHorizontalTriggered(const FInputActionValue& ActionValue);

	void OnSetRotateCameraStarted();
	void OnSetRotateCameraReleased();

private:
	FVector CachedDestination;

	bool bCanRotateCamera;
	bool bIsManualMoving;

	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed

	// Rotate the pawn to look at the destination
	void LookAtDestination(const FVector& Destination) const;
};
