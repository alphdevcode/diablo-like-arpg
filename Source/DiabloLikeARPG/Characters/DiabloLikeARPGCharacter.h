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

	// void HandleRespawn();

	UFUNCTION(BlueprintPure)
	bool IsDead() const;
	
	FORCEINLINE class UStatsComponent* GetStatsComponent() const { return StatsComponent; }
	FORCEINLINE class UAbilitiesComponent* GetAbilitiesComponent() const { return AbilitiesComponent; }

private:
	
	FTimerHandle InteractionTimerHandle;
	FTimerHandle DestroyActorTimerHandle;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStatsComponent* StatsComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UAbilitiesComponent* AbilitiesComponent;
	
	/** Set upon interaction, will be cleared after interaction finished **/
	IInteractableInterface* CurrentInteractable;
	/** Set upon interaction, won't be cleared after interaction finished **/
	IInteractableInterface* TargetInteractable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanRespawn = false;

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
