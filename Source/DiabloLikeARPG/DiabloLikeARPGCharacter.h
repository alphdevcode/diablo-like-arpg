// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractorInterface.h"
#include "InteractableInterface.h"
#include "GameFramework/Character.h"
#include "DiabloLikeARPGCharacter.generated.h"

class AAbility;
class AAbilityAttack;
class IInteractableInterface;

UCLASS(Blueprintable)
class ADiabloLikeARPGCharacter : public ACharacter, public IInteractorInterface
{
	GENERATED_BODY()

public:
	ADiabloLikeARPGCharacter();

	virtual void BeginPlay() override;

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;
	
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent,
	                         AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UPROPERTY(VisibleAnywhere)
	class UStatsComponent* StatsComponent;

	UPROPERTY(VisibleAnywhere)
	class UAbilitiesComponent* AbilitiesComponent;

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	FTimerHandle InteractionTimerHandle;

protected:
	IInteractableInterface* CurrentInteractable;

	void CheckForInteractions();

public:
	virtual void SetInteractableTarget(IInteractableInterface* Interactable) override;
	virtual float GetInteractionRange() override;

	void ActivateAttackAbility() const;
};
