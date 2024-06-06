// Copyright 2024 AlphDevCode. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DiabloLikeARPGGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerEntersBossRadius, AActor*, PlayerActor, AActor*, BossEnemyActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerExitsBossRadius, AActor*, PlayerActor, AActor*, BossEnemyActor);

/**
 * 
 */
UCLASS()
class DIABLOLIKEARPG_API ADiabloLikeARPGGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category="Boss")
	FOnPlayerEntersBossRadius OnPlayerEntersBossRadius;

	UPROPERTY(BlueprintAssignable, Category="Boss")
	FOnPlayerExitsBossRadius OnPlayerExitsBossRadius;
};
