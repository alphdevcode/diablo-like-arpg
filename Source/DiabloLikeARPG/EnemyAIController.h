// Copyright 2024 AlphDevCode. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOLIKEARPG_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
public:
	AEnemyAIController();
	
protected:
	UFUNCTION()
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	void Initialize();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	class UAIPerceptionComponent* AIPerceptionComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	class UAIPerceptionStimuliSourceComponent* AIPerceptionStimuliSourceComponent;

	UPROPERTY()
	class ADiabloLikeARPGCharacter* ControlledCharacter;

private:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;

	bool bShouldLookForPlayer = true;

	/** If true, the enemy will automatically chase the player without needing to see him first */
	bool bAutoChasePlayer = false;

	const FName TargetEnemyKey = "TargetEnemy";
	const FName IdealRangeKey = "IdealRange";

public:
	virtual void GameHasEnded(AActor* EndGameFocus, bool bIsWinner) override;
	
	UFUNCTION(BlueprintPure)
	AActor* GetAttackTarget() const;
	
	UFUNCTION(BlueprintPure)
	virtual float GetIdealRange() const;

	void SetAttackTarget(AActor* NewAttackTarget);
	void SetAutoChasePlayer(const bool NewAutoChasePlayer);
};
