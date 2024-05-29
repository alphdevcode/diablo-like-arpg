// Copyright 2024 AlphDevCode. All Rights Reserved.


#include "AbilityRangedProjectile.h"

#include "DiabloLikeARPG/DiabloLikeARPGAnimInstance.h"
#include "DiabloLikeARPG/Characters/DiabloLikeARPGCharacter.h"
#include "GameFramework/Character.h"

void AAbilityRangedProjectile::Initialize()
{
	Super::Initialize();
}

void AAbilityRangedProjectile::BeforeActivateAbility()
{
	if(const ADiabloLikeARPGCharacter* Character = Cast<ADiabloLikeARPGCharacter>(Caster); Character != nullptr)
	{
		Target = Character->GetTargetInteractableActor();
	}
}

void AAbilityRangedProjectile::AbilityActivated()
{
	Super::AbilityActivated();

	CasterAnimInstance->OnAnimNotifyAttackPeak
		.AddDynamic(this, &AAbilityRangedProjectile::HandleAbilityEffectsSpawning);
}

void AAbilityRangedProjectile::HandleAbilityEffectsSpawning()
{
	SpawnAbilityEffectsWithLocation(Caster->GetMesh()->GetSocketLocation(AbilityEffectsSocket));
	CasterAnimInstance->OnAnimNotifyAttackPeak
		.RemoveDynamic(this, &AAbilityRangedProjectile::HandleAbilityEffectsSpawning);
}
