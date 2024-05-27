// Copyright 2024 AlphDevCode. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Ability.h"
#include "AbilityAttack.generated.h"

class UDiabloLikeARPGAnimInstance;
/**
 * 
 */
UCLASS()
class DIABLOLIKEARPG_API AAbilityAttack : public AAbility
{
	GENERATED_BODY()

protected:
	virtual void Initialize() override;
	virtual void AbilityActivated() override;

	void HandleAttackCombo();

	UFUNCTION()
	void ComboAttackSave();

	UFUNCTION()
	virtual void ResetAbility() override;

	UFUNCTION()
	void HandleAbilityEffectsSpawning();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UParticleSystem* ImpactFX;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TArray<UAnimMontage*> AttackAnimMontages;
	
	UPROPERTY(BlueprintReadWrite)
	bool bSaveAttack = false;

	UPROPERTY(BlueprintReadWrite)
	int32 AttackCount = 0;

	UPROPERTY(BlueprintReadWrite)
	bool bIsAttacking = false;

	UPROPERTY(BlueprintReadWrite)
	bool bShouldLookAtTarget;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	double MeleeSpeed = 1.2f;
	
	void RotateToFaceTarget();

public:
	virtual void Tick(float DeltaTime) override;
};
