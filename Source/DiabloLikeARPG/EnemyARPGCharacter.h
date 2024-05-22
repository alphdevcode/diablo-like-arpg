// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DiabloLikeARPGCharacter.h"
#include "EnemyARPGCharacter.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOLIKEARPG_API AEnemyARPGCharacter : public ADiabloLikeARPGCharacter, public IInteractableInterface
{
	GENERATED_BODY()

public:
	virtual void NotifyActorOnClicked(FKey ButtonPressed) override;
	
	// IInteractableInterface
	virtual void Interact(ACharacter* InteractorCharacter) override;
	virtual const AActor* GetInteractableActor() const override;
};
