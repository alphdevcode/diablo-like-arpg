// Copyright 2024 AlphDevCode. All Rights Reserved.


#include "Actors/GameModes/SoloARPGGameMode.h"

#include "EngineUtils.h"
#include "AI/EnemyAIController.h"
#include "Actors/EnemyBossSpawner.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


ASoloARPGGameMode::ASoloARPGGameMode()
{
	BossFightDelay = 5.f;
}

void ASoloARPGGameMode::BeginPlay()
{
	Super::BeginPlay();

	EnemiesLeft = GetEnemiesCountInWorld();
	BossSpawners = FindAllBossSpawners();
	ScheduleBossFight();
}

void ASoloARPGGameMode::PawnDied(APawn* PawnKilled)
{
	Super::PawnDied(PawnKilled);

	APlayerController* PlayerController = nullptr;
	if(const ACharacter* Character = Cast<ACharacter>(PawnKilled))
	{
		PlayerController = Cast<APlayerController>(Character->GetController());
	}

	if (PlayerController != nullptr)
	{
		EndGame(false);
		PlayerDied(PlayerController); 
	}
	else
	{
		EnemiesLeft--;
		if (EnemiesLeft <= 0)
		{
			EndGame(true);
		}
	}
}

int16 ASoloARPGGameMode::GetEnemiesCountInWorld() const
{
	TArray<AActor*> EnemiesInWorld;
	UGameplayStatics::GetAllActorsOfClass(this, AEnemyAIController::StaticClass(),
										  EnemiesInWorld);

	return EnemiesInWorld.Num();
}

TArray<AEnemyBossSpawner*> ASoloARPGGameMode::FindAllBossSpawners() const
{
	TArray<AActor*> ActorsToFind;
	UGameplayStatics::GetAllActorsOfClass(this, AEnemyBossSpawner::StaticClass(), ActorsToFind);

	TArray<AEnemyBossSpawner*> FoundBossSpawners;
	for (AActor* Actor : ActorsToFind)
	{
		if(AEnemyBossSpawner* FoundBossSpawner = Cast<AEnemyBossSpawner>(Actor))
		{
			FoundBossSpawners.Add(FoundBossSpawner);
		}
	}
	return FoundBossSpawners;
}

void ASoloARPGGameMode::ScheduleBossFight()
{
	if (BossSpawners.Num() > 0)
	{
		for (AEnemyBossSpawner* BossSpawner : BossSpawners)
		{
			FTimerHandle UnusedTimerHandle;
			GetWorldTimerManager().SetTimer(UnusedTimerHandle, BossSpawner,
				&AEnemyBossSpawner::HandleEnemiesSpawning, BossFightDelay, false);
		}
	}
}

void ASoloARPGGameMode::EndGame(bool bIsPlayerWinner)
{
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		const bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
	}
}
