// Copyright 2024 AlphDevCode. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilityWithSingleAnimationBase.h"
#include "AbilityHealSelf.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOLIKEARPG_API AAbilityHealSelf : public AAbilityWithSingleAnimationBase
{
	GENERATED_BODY()

protected:
	virtual void Initialize() override;
	virtual void AbilityActivated() override;
};
