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

	void HandleEnemiesSpawning();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TArray<TSubclassOf<AEnemyARPGCharacter>> EnemyClasses;

	/** Number of enemies to spawn each interval */
	UPROPERTY(EditAnywhere, Category = "Spawning")
	int32 SpawnCount;

	/** Spawn interval in seconds */
	UPROPERTY(EditAnywhere, Category = "Spawning")
	float SpawnDelay;
	
	/** Whether to loop spawning over a given time or not */
	UPROPERTY(EditAnywhere, Category = "Spawning")
	bool bLoop;

	/** Whether to loop spawning over a given time or not */
	UPROPERTY(EditAnywhere, Category = "Spawning")
	bool bAutoStartSpawning;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	ACharacter* PlayerCharacter;
	
	void SpawnEnemies();

	FTimerHandle SpawnTimerHandle;
};
