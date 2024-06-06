// Copyright 2024 AlphDevCode. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilityEffect.h"
#include "AbilityEffectFX.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOLIKEARPG_API AAbilityEffectFX : public AAbilityEffect
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UParticleSystem* AbilityFX;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FVector SpawnOffset;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool bShouldAttachToCaster = false;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (EditCondition = "bShouldAttachToCaster == true", EditConditionHides))
	FName AttachPointName;

private:
	void DestroyEffect();
	
};
