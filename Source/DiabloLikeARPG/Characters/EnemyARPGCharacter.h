// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DiabloLikeARPGCharacter.h"
#include "DiabloLikeARPG/InteractionSystem/InteractableInterface.h"
#include "EnemyARPGCharacter.generated.h"

class UWidgetComponent;
/**
 * 
 */
UCLASS()
class DIABLOLIKEARPG_API AEnemyARPGCharacter : public ADiabloLikeARPGCharacter, public IInteractableInterface
{
	GENERATED_BODY()
public:
	AEnemyARPGCharacter();

protected:
	void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Overlay")
	UMaterialInterface* OverlayMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Health")
	UWidgetComponent* HealthBar;

public:
	virtual void NotifyActorOnClicked(FKey ButtonPressed) override;

	virtual void NotifyActorBeginCursorOver() override;
	virtual void NotifyActorEndCursorOver() override;
	
	// IInteractableInterface
	virtual void Interact(ACharacter* InteractorCharacter) override;
	virtual const AActor* GetInteractableActor() const override;
};