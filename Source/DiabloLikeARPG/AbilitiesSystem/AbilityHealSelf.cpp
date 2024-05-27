// Copyright 2024 AlphDevCode. All Rights Reserved.


#include "AbilityHealSelf.h"

#include "GameFramework/Character.h"

void AAbilityHealSelf::Initialize()
{
	Super::Initialize();
}

void AAbilityHealSelf::AbilityActivated()
{
	if(HealAnimMontage == nullptr)
	{
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red,
				TEXT("HealAnimMontage is not valid!"));
		return;
	}
	Caster->PlayAnimMontage(HealAnimMontage);

	SpawnAbilityEffects();
}


