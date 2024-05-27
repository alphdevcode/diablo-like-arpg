// Copyright 2024 AlphDevCode. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Ability.h"
#include "AbilityHealSelf.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOLIKEARPG_API AAbilityHealSelf : public AAbility
{
	GENERATED_BODY()

protected:
	virtual void Initialize() override;
	virtual void AbilityActivated() override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UParticleSystem* HealFX;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UAnimMontage* HealAnimMontage;
};
