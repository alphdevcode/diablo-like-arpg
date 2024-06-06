// Copyright 2024 AlphDevCode. All Rights Reserved.


#include "AbilityRangedAOE.h"

#include "AbilityAOE.h"
#include "DiabloLikeARPG/DiabloLikeARPGAnimInstance.h"
#include "GameFramework/Character.h"

void AAbilityRangedAOE::Initialize()
{
	Super::Initialize();
}

void AAbilityRangedAOE::AbilityActivated()
{
	Super::AbilityActivated();

	CasterAnimInstance->OnAnimNotifyAttackPeak
		.AddDynamic(this, &AAbilityRangedAOE::HandleAbilityEffectsSpawning);
}

void AAbilityRangedAOE::HandleAbilityEffectsSpawning()
{
	if(AController* CasterController = Caster->GetController(); CasterController != nullptr)
	{
		CasterController->StopMovement();
	}
	
	SpawnAbilityEffectsWithLocation(Target->GetActorLocation() + EffectsSpawnOffset);
	CasterAnimInstance->OnAnimNotifyAttackPeak
		.RemoveDynamic(this, &AAbilityRangedAOE::HandleAbilityEffectsSpawning);
}