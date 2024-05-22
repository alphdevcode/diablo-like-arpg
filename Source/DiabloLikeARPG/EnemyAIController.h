// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "InteractableInterface.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOLIKEARPG_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void FindPawn(const APawn* TargetPawn);

	UPROPERTY(EditAnywhere)
	float FindPawnTimerRate = .5f;

	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior; 
	
};
