// Copyright 2024 AlphDevCode. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class AEnemyARPGCharacter;

UCLASS()
class DIABLOLIKEARPG_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawner();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TArray<TSubclassOf<AEnemyARPGCharacter>> EnemyClasses;

	/** Spawn interval in seconds */
	UPROPERTY(EditAnywhere, Category = "Spawning")
	float SpawnInterval;

	/** Number of enemies to spawn each interval */
	UPROPERTY(EditAnywhere, Category = "Spawning")
	int32 SpawnCount;

	UPROPERTY()
	ACharacter* PlayerCharacter;
	
	void SpawnEnemies();

	FTimerHandle SpawnTimerHandle;
};
