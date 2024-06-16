// Copyright 2024 AlphDevCode. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilityWithSingleAnimationBase.h"
#include "AbilityRangedAOE.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOLIKEARPG_API AAbilityRangedAOE : public AAbilityWithSingleAnimationBase
{
	GENERATED_BODY()

protected:
	virtual void Initialize() override;
	virtual void AbilityActivated() override;

	UFUNCTION()
	void HandleAbilityEffectsSpawning();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FVector EffectsSpawnOffset;
};
