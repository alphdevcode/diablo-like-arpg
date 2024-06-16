// Copyright 2024 AlphDevCode. All Rights Reserved.


#include "Actors/Abilities/AbilityAOE.h"

#include "Animation/DiabloLikeARPGAnimInstance.h"
#include "GameFramework/Character.h"

void AAbilityAOE::Initialize()
{
	Super::Initialize();
}

void AAbilityAOE::AbilityActivated()
{
	Super::AbilityActivated();

	CasterAnimInstance->OnAnimNotifyAttackPeak
		.AddDynamic(this, &AAbilityAOE::HandleAbilityEffectsSpawning);
}

void AAbilityAOE::HandleAbilityEffectsSpawning()
{
	if(AController* CasterController = Caster->GetController(); CasterController != nullptr)
	{
		CasterController->StopMovement();
	}

	SpawnAbilityEffectsWithLocation(Caster->GetMesh()->GetSocketLocation(AbilityEffectsSocket) - FVector(0, 0, 15));
	CasterAnimInstance->OnAnimNotifyAttackPeak
		.RemoveDynamic(this, &AAbilityAOE::HandleAbilityEffectsSpawning);
}
