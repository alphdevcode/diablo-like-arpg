// Copyright 2024 AlphDevCode. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DiabloLikeARPGAnimInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnimNotifySaveAttack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnimNotifyResetCombo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnimNotifyAttackPeak);

/**
 * 
 */
UCLASS()
class DIABLOLIKEARPG_API UDiabloLikeARPGAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Abilities")
	FOnAnimNotifySaveAttack OnAnimNotifySaveAttack;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Abilities")
	FOnAnimNotifyResetCombo OnAnimNotifyResetCombo;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Abilities")
	FOnAnimNotifyAttackPeak OnAnimNotifyAttackPeak;
};
