// Copyright 2024 AlphDevCode. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilityEffect.h"
#include "AbilityEffectRangedProjectile.generated.h"

UCLASS()
class DIABLOLIKEARPG_API AAbilityEffectRangedProjectile : public AAbilityEffect
{
	GENERATED_BODY()

protected:
	virtual void OnActivate() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AProjectileBase> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	float BaseDamage = 10.f;
};
