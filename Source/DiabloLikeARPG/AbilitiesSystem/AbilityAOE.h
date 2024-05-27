// Copyright 2024 AlphDevCode. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Ability.h"
#include "AbilityAOE.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOLIKEARPG_API AAbilityAOE : public AAbility
{
	GENERATED_BODY()

protected:
	virtual void Initialize() override;
	virtual void AbilityActivated() override;

	UFUNCTION()
	void HandleAbilityEffectsSpawning();
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UAnimMontage* AbilityAnimMontage;

private:
	const FName AbilityEffectsSocket = FName("FX_Sword_Top");
};
