// Copyright 2024 AlphDevCode. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DiabloLikeARPG/InteractionSystem/InteractorInterface.h"
#include "GameFramework/Character.h"
#include "DiabloLikeARPGCharacter.generated.h"

class UNiagaraSystem;
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

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaSeconds) override;
	
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent,
	                         AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStatsComponent* StatsComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UAbilitiesComponent* AbilitiesComponent;

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	FTimerHandle InteractionTimerHandle;
	FTimerHandle DestroyActorTimerHandle;

protected:
	/** Set upon interaction, will be cleared after interaction finished **/
	IInteractableInterface* CurrentInteractable;
	/** Set upon interaction, won't be cleared after interaction finished **/
	IInteractableInterface* TargetInteractable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* HitAnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* DestroyFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USoundBase* DestroySound;

	void CheckForInteractions();

	void DestroyCharacter();

public:
	virtual float GetInteractionRange() override;
	virtual void SetTargetInteractable(IInteractableInterface* Interactable) override;

	void SetCurrentInteractable(IInteractableInterface* Interactable);

	void ActivatePrimaryAttackAbility() const;

	AActor* GetTargetInteractableActor() const;
};
