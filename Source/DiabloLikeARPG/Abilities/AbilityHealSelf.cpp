// Copyright 2024 AlphDevCode. All Rights Reserved.


#include "AbilityHealSelf.h"

void AAbilityHealSelf::Initialize()
{
	Super::Initialize();
}

void AAbilityHealSelf::AbilityActivated()
{
	Super::AbilityActivated();
	
	SpawnAbilityEffects();
}


