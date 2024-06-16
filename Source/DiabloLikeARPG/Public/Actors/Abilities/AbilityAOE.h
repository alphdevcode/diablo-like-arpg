// Copyright 2024 AlphDevCode. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilityWithSingleAnimationBase.h"
#include "AbilityAOE.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOLIKEARPG_API AAbilityAOE : public AAbilityWithSingleAnimationBase
{
	GENERATED_BODY()

protected:
	virtual void Initialize() override;
	virtual void AbilityActivated() override;

	UFUNCTION()
	void HandleAbilityEffectsSpawning();

private:
	const FName AbilityEffectsSocket = FName("FX_Sword_Top");
};
