// Copyright 2024 AlphDevCode. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DiabloLikeARPGCharacter.h"
#include "ACtors/InteractionSystem/InteractableInterface.h"
#include "EnemyARPGCharacter.generated.h"

class UAIPerceptionComponent;
class UWidgetComponent;
class AEnemyARPGCharacter;

/**
 * 
 */
UCLASS()
class DIABLOLIKEARPG_API AEnemyARPGCharacter : public ADiabloLikeARPGCharacter, public IInteractableInterface
{
	GENERATED_BODY()
public:
	AEnemyARPGCharacter();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent,
							 AController* EventInstigator, AActor* DamageCauser) override;


protected:
	virtual void BeginPlay() override;
	
	IInteractorInterface* GetPlayerInteractor() const;

	UFUNCTION(BlueprintCallable, Category="Abilities")
	void Attack();

	UFUNCTION(BlueprintCallable, Category="Abilities")
	void ActivateMainAbility();

	/** The radius in which the enemy can interact with actors (e.g. attack the player) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction")
	float InteractionRadius = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Overlay")
	UMaterialInterface* OverlayMaterial;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Health")
	UWidgetComponent* HealthBar;

	UPROPERTY(EditDefaultsOnly)
	FName EnemyName;

public:
	virtual void NotifyActorOnClicked(FKey ButtonPressed) override;

	virtual void NotifyActorBeginCursorOver() override;
	virtual void NotifyActorEndCursorOver() override;
	
	// IInteractableInterface
	virtual void Interact(ACharacter* InteractorCharacter) override;
	virtual AActor* GetInteractableActor() override;

	UFUNCTION(BlueprintPure)
	FName GetEnemyName() const;
};
