// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilityEffect.generated.h"

class AAbility;

UCLASS()
class DIABLOLIKEARPG_API AAbilityEffect : public AActor
{
	GENERATED_BODY() 
	
public:	
	// Sets default values for this actor's properties
	AAbilityEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	AAbility* ParentAbility;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetPatentAbility(AAbility* Ability);
};
