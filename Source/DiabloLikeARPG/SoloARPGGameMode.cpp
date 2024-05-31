// Copyright 2024 AlphDevCode. All Rights Reserved.


#include "SoloARPGGameMode.h"

#include "DiabloLikeARPGPlayerController.h"
#include "EnemyAIController.h"
#include "EngineUtils.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


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

void ASoloARPGGameMode::BeginPlay()
{
	Super::BeginPlay();

	EnemiesLeft = GetEnemiesCountInWorld();
}

int16 ASoloARPGGameMode::GetEnemiesCountInWorld() const
{
	TArray<AActor*> EnemiesInWorld;
	UGameplayStatics::GetAllActorsOfClass(this, AEnemyAIController::StaticClass(),
										  EnemiesInWorld);

	return EnemiesInWorld.Num();
}

void ASoloARPGGameMode::EndGame(bool bIsPlayerWinner)
{
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		const bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
	}
}
