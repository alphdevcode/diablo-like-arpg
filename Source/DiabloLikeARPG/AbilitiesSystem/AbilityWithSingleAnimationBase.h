// Copyright 2024 AlphDevCode. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Ability.h"
#include "AbilityWithSingleAnimationBase.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOLIKEARPG_API AAbilityWithSingleAnimationBase : public AAbility
{
	GENERATED_BODY()

protected:
	virtual void AbilityActivated() override;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UAnimMontage* AbilityAnimMontage;
	
};
