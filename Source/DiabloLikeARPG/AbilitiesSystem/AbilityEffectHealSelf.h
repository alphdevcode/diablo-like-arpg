// Copyright 2024 AlphDevCode. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilityEffect.h"
#include "AbilityEffectHealSelf.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOLIKEARPG_API AAbilityEffectHealSelf : public AAbilityEffect
{
	GENERATED_BODY()

protected:
	virtual void OnActivate() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect", meta = (AllowPrivate))
	int32 HealAmount = 10;

private:
	void DestroyEffect();
};
