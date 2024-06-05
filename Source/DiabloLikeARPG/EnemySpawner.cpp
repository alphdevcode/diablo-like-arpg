// Copyright 2024 AlphDevCode. All Rights Reserved.


#include "EnemySpawner.h"

#include "EnemyAIController.h"
#include "Characters/EnemyARPGCharacter.h"
#include "Kismet/GameplayStatics.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;

	SpawnInterval = 5.0f;
	SpawnCount = 5;
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);

	GetWorldTimerManager().SetTimer(SpawnTimerHandle,
	                                this, &AEnemySpawner::SpawnEnemies, SpawnInterval, true);
}

void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemySpawner::SpawnEnemies()
{
	for (int16 i = 0; i < SpawnCount; i++)
	{
		// Randomly select an enemy class
		TSubclassOf<AEnemyARPGCharacter> SelectedEnemyClass = EnemyClasses[FMath::RandRange(0, EnemyClasses.Num() - 1)];

		if (SelectedEnemyClass == nullptr)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
				                                 "Can not spawn enemy. Enemy class is null!");
			continue;
		}

		const AEnemyARPGCharacter* EnemyCharacter = GetWorld()->SpawnActor<AEnemyARPGCharacter>(
			SelectedEnemyClass, GetActorLocation(), GetActorRotation());
		
		if (EnemyCharacter == nullptr)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
												 "Can not spawn enemy. Enemy character is null!");
			continue;
		}
		
		if (AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(EnemyCharacter->Controller))
		{
			EnemyAIController->SetAutoChasePlayer(true);
		}
	}
}
