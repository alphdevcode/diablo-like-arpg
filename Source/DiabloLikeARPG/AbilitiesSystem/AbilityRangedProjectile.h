// Copyright 2024 AlphDevCode. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilityWithSingleAnimationBase.h"
#include "AbilityRangedProjectile.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOLIKEARPG_API AAbilityRangedProjectile : public AAbilityWithSingleAnimationBase
{
	GENERATED_BODY()

protected:
	virtual void Initialize() override;
	virtual void BeforeActivateAbility() override;
	virtual void AbilityActivated() override;

	UFUNCTION()
	virtual void ResetAbility() override;
	
	UFUNCTION()
	void HandleAbilityEffectsSpawning();

	UPROPERTY(BlueprintReadWrite)
	bool bShouldLookAtTarget;

private:
	const FName AbilityEffectsSocket = FName("FX_Sword_Bottom");

	void RotateToFaceTarget();
	
public:
	virtual void Tick(float DeltaTime) override;
};
