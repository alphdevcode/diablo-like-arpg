// Copyright 2024 AlphDevCode. All Rights Reserved.

#include "AbilityAttack.h"

#include "DiabloLikeARPG/DiabloLikeARPGAnimInstance.h"
#include "DiabloLikeARPG/DiabloLikeARPGPlayerController.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void AAbilityAttack::BeginPlay()
{
	Super::BeginPlay();
}

void AAbilityAttack::Initialize()
{
	Super::Initialize();

	OnAbilityActivated.AddDynamic(this, &AAbilityAttack::AbilityActivated);

	if ((AnimInstance = Cast<UDiabloLikeARPGAnimInstance>(Caster->GetMesh()->GetAnimInstance())))
	{
		AnimInstance->OnAnimNotifySaveAttack.AddDynamic(this, &AAbilityAttack::ComboAttackSave);
		AnimInstance->OnAnimNotifyResetCombo.AddDynamic(this, &AAbilityAttack::ResetCombo);
		AnimInstance->OnAnimNotifyAttackPeak.AddDynamic(this, &AAbilityAttack::HandleAbilityEffectsSpawning);
	}
}

void AAbilityAttack::AbilityActivated()
{
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
	}
	else
	{
		bShouldLookAtTarget = false;
		bIsAttacking = false;
		AttackCount = 0;
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red,
			                                 TEXT("AttackAnimMontages is not valid!"));
		return;
	}

	switch (AttackCount)
	{
	case 0:
		AttackCount = 1;
		break;
	case 1:
		AttackCount = 2;
		break;
	case 2:
	default:
		AttackCount = 0;
		break;
	}
}

void AAbilityAttack::ComboAttackSave()
{
	if (!bSaveAttack) return;

	bSaveAttack = false;
	HandleAttackCombo();
}

void AAbilityAttack::ResetCombo()
{
	AttackCount = 0;
	bSaveAttack = false;
	bIsAttacking = false;
	bShouldLookAtTarget = false;
}

void AAbilityAttack::HandleAbilityEffectsSpawning()
{
	if (const ACharacter* TargetCharacter = Cast<ACharacter>(Target))
	{
		if (ImpactFX != nullptr)
		{
			// play particle system
			UGameplayStatics::SpawnEmitterAtLocation(this,
			                                         ImpactFX,
			                                         TargetCharacter->GetMesh()->GetSocketLocation("Impact"));
		}
		SpawnAbilityEffects();
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 2.f,
			                                 FColor::Red,
			                                 TEXT("Trying to spawn abilities effect with a nullptr Target"));
	}
	bShouldLookAtTarget = false;
}

void AAbilityAttack::RotateToFaceTarget()
{
	if (Caster != nullptr && Target != nullptr)
	{
		if (ADiabloLikeARPGPlayerController* CasterController = Cast<ADiabloLikeARPGPlayerController>(
			Caster->GetController()))
		{
			CasterController->LookAtDestination(Target->GetActorLocation());
		}
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
