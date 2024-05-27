// Copyright 2024 AlphDevCode. All Rights Reserved.

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
	virtual void BeginPlay() override;

	virtual void OnActivate();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	AAbility* ParentAbility;

public:	
	virtual void Tick(float DeltaTime) override;

	void SetParentAbility(AAbility* Ability);
};
