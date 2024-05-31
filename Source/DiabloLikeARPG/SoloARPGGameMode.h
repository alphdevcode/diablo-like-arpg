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
	
protected:
	virtual void BeginPlay() override;

	virtual void PawnDied(APawn* PawnKilled) override;

private:
	void EndGame(bool bIsPlayerWinner);
	int16 GetEnemiesCountInWorld() const;

	int16 EnemiesLeft;
};
