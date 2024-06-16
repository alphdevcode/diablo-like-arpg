// Copyright 2024 AlphDevCode. All Rights Reserved.


#include "Actors/Abilities/AbilityRangedAOE.h"

#include "DiabloLikeARPG/Public/Animation/DiabloLikeARPGAnimInstance.h"
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