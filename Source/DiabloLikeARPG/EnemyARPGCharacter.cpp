// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyARPGCharacter.h"

#include "Kismet/GameplayStatics.h"

void AEnemyARPGCharacter::NotifyActorOnClicked(FKey ButtonPressed)
{
	Super::NotifyActorOnClicked(ButtonPressed);

	if(ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0))
	{
		if(IInteractorInterface* Interactor = Cast<IInteractorInterface>(PlayerCharacter))
		{
			Interactor->SetInteractableTarget(Cast<IInteractableInterface>(this));
		}
	}
}

void AEnemyARPGCharacter::NotifyActorBeginCursorOver()
{
	Super::NotifyActorBeginCursorOver();
	GetMesh()->SetOverlayMaterial(OverlayMaterial);
}

void AEnemyARPGCharacter::NotifyActorEndCursorOver()
{
	Super::NotifyActorEndCursorOver();
	GetMesh()->SetOverlayMaterial(nullptr);
}

void AEnemyARPGCharacter::Interact(ACharacter* InteractorCharacter)
{
	if(ADiabloLikeARPGCharacter* DiabloLikeARPGCharacter = Cast<ADiabloLikeARPGCharacter>(InteractorCharacter))
	{
		DiabloLikeARPGCharacter->ActivatePrimaryAttackAbility();
	}
}

const AActor* AEnemyARPGCharacter::GetInteractableActor() const
{
	return this;
}