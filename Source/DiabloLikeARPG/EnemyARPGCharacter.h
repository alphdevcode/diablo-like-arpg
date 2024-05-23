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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Overlay")
	UMaterialInterface* OverlayMaterial;

public:
	virtual void NotifyActorOnClicked(FKey ButtonPressed) override;

	virtual void NotifyActorBeginCursorOver() override;
	virtual void NotifyActorEndCursorOver() override;
	
	// IInteractableInterface
	virtual void Interact(ACharacter* InteractorCharacter) override;
	virtual const AActor* GetInteractableActor() const override;
};
