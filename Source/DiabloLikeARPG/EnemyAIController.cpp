// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"

#include "DiabloLikeARPGCharacter.h"
#include "InteractorInterface.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"


void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	if (AIBehavior)
	{
		RunBehaviorTree(AIBehavior);
	}
}

void AEnemyAIController::NotifyActorOnClicked(FKey ButtonPressed)
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

void AEnemyAIController::Interact(ACharacter* InteractorCharacter)
{
	if(ADiabloLikeARPGCharacter* DiabloLikeARPGCharacter = Cast<ADiabloLikeARPGCharacter>(InteractorCharacter))
	{
		DiabloLikeARPGCharacter->ActivateAttackAbility();
	}
}

const AActor* AEnemyAIController::GetInteractableActor() const
{
	return this;
}
