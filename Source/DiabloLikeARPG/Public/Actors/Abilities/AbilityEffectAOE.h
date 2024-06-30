// Copyright 2024 AlphDevCode. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilityEffect.h"
#include "AbilityEffectAOE.generated.h"

class USphereComponent;
/**
 * 
 */
UCLASS()
class DIABLOLIKEARPG_API AAbilityEffectAOE : public AAbilityEffect
{
	GENERATED_BODY()

public:
	AAbilityEffectAOE();
	
protected:
	virtual void OnActivate() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	float BaseDamage = 70.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect", meta=(AllowPrivateAccess = "true"))
	TSubclassOf<AActor> ClassFilter;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess = "true"))
	USphereComponent* SphereCollision;
};
