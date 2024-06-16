// Copyright 2024 AlphDevCode. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DiabloLikeARPGGameMode.h"
#include "SoloARPGGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOLIKEARPG_API ASoloARPGGameMode : public ADiabloLikeARPGGameMode
{
	GENERATED_BODY()

public:
	ASoloARPGGameMode();
	
protected:
	virtual void BeginPlay() override;

	virtual void PawnDied(APawn* PawnKilled) override;

	UPROPERTY(EditDefaultsOnly, Category="Boss Fight")
	float BossFightDelay;

private:
	void EndGame(bool bIsPlayerWinner);
	int16 GetEnemiesCountInWorld() const;
	TArray<class AEnemyBossSpawner*> FindAllBossSpawners() const;
	void ScheduleBossFight();

	int16 EnemiesLeft;

	TArray<class AEnemyBossSpawner*> BossSpawners;
};
