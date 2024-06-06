// Copyright 2024 AlphDevCode. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilityEffect.h"
#include "AbilityEffectSound.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOLIKEARPG_API AAbilityEffectSound : public AAbilityEffect
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	USoundBase* AbilitySound;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool bShouldAttachToCaster = false;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (EditCondition = "bShouldAttachToCaster == true", EditConditionHides))
	FName AttachPointName;

private:
	void DestroyEffect();
};
