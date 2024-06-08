// Copyright 2024 AlphDevCode. All Rights Reserved.


#include "EnemySpawner.h"

#include "Components/CapsuleComponent.h"
#include "DiabloLikeARPG/EnemyAIController.h"
#include "DiabloLikeARPG/Characters/EnemyARPGCharacter.h"
#include "Kismet/GameplayStatics.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;

	bLoop = true;
	SpawnDelay = 5.0f;
	SpawnCount = 5;
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);

	if (bAutoStartSpawning)
	{
		HandleEnemiesSpawning();
	}
}

void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemySpawner::HandleEnemiesSpawning()
{
	GetWorldTimerManager().SetTimer(SpawnTimerHandle,
	        this, &AEnemySpawner::SpawnEnemies, SpawnDelay, bLoop);
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

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		const AEnemyARPGCharacter* EnemyCharacter = GetWorld()->SpawnActor<AEnemyARPGCharacter>(
			SelectedEnemyClass, GetActorLocation(), GetActorRotation(), SpawnParameters);

		if (EnemyCharacter == nullptr)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
				"Can not spawn enemy. Enemy character returned null!");
			continue;
		}

		// Manually address bug where spawned actors will get their CollisionResponse overridden.
		// This is needed so they can get hit by attacks
		EnemyCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic,
			ECollisionResponse::ECR_Block);

		if (AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(EnemyCharacter->Controller))
		{
			EnemyAIController->SetAutoChasePlayer(true);
		}
	}
}