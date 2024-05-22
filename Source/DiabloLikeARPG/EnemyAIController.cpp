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


