// Copyright 2024 AlphDevCode. All Rights Reserved.

#include "Actors/Abilities/AbilityAttack.h"

#include "Libraries/ActorFunctionLibrary.h"
#include "Animation/DiabloLikeARPGAnimInstance.h"
#include "Actors/Characters/DiabloLikeARPGCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Libraries/Logger.h"


void AAbilityAttack::Initialize()
{
	Super::Initialize();

	if (CasterAnimInstance != nullptr)
	{
		CasterAnimInstance->OnAnimNotifySaveAttack.AddDynamic(this, &AAbilityAttack::ComboAttackSave);
		CasterAnimInstance->OnAnimNotifyResetCombo.AddDynamic(this, &AAbilityAttack::ResetAbility);
	}
}

void AAbilityAttack::AbilityActivated()
{
	Super::AbilityActivated();

	bShouldLookAtTarget = true;
	if (bIsAttacking)
	{
		bSaveAttack = true;
	}
	else
	{
		bIsAttacking = true;
		HandleAttackCombo();
	}
}

void AAbilityAttack::HandleAttackCombo()
{
	if (AttackAnimMontages.IsValidIndex(AttackCount))
	{
		Caster->PlayAnimMontage(AttackAnimMontages[AttackCount], MeleeSpeed);
		if (CasterAnimInstance != nullptr)
		{
			CasterAnimInstance->OnAnimNotifyAttackPeak.AddDynamic(this, &AAbilityAttack::HandleAbilityEffectsSpawning);
		}

		if (CastSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, CastSound, Caster->GetActorLocation());
		}
	}
	else
	{
		ResetAbility();
		LOG_ERROR(TEXT("AttackAnimMontage is not valid!"));
		return;
	}

	// Set the attack count to the next attack in the combo, if it's the last one set it back to zero
	AttackCount = (AttackCount + 1) % AttackAnimMontages.Num();
}

void AAbilityAttack::ComboAttackSave()
{
	if (!bSaveAttack) return;

	bSaveAttack = false;
	HandleAttackCombo();
}

void AAbilityAttack::ResetAbility()
{
	AttackCount = 0;
	bSaveAttack = false;
	bIsAttacking = false;
	bShouldLookAtTarget = false;
}

void AAbilityAttack::HandleAbilityEffectsSpawning()
{
	if (const ADiabloLikeARPGCharacter* TargetCharacter = Cast<ADiabloLikeARPGCharacter>(Target))
	{
		// If the target is already dead, don't spawn the effects
		if (TargetCharacter->IsDead()) return;

		if (ImpactFX != nullptr)
		{
			// play particle system
			UGameplayStatics::SpawnEmitterAtLocation(this,
			                                         ImpactFX, TargetCharacter->GetMesh()->GetSocketLocation("Impact"),
			                                         FRotator::ZeroRotator, FVector(1), true,
			                                         EPSCPoolMethod::AutoRelease);
		}
		SpawnAbilityEffects();
		CasterAnimInstance->OnAnimNotifyAttackPeak.RemoveDynamic(this, &AAbilityAttack::HandleAbilityEffectsSpawning);
	}
	else
	{
		LOG_ERROR(TEXT("Trying to spawn abilities effect with a nullptr Target"));
	}
	bShouldLookAtTarget = false;
}

void AAbilityAttack::RotateToFaceTarget()
{
	if (Caster != nullptr && Target != nullptr)
	{
		UActorFunctionLibrary::LookAtDestination(Caster, Target->GetActorLocation());
	}
}

void AAbilityAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShouldLookAtTarget)
	{
		RotateToFaceTarget();
	}
}
