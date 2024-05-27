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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	class UAIPerceptionComponent* AIPerceptionComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	class UAIPerceptionStimuliSourceComponent* AIPerceptionStimuliSourceComponent;

private:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior; 

	const FName TargetEnemyKey = "TargetEnemy";
};
