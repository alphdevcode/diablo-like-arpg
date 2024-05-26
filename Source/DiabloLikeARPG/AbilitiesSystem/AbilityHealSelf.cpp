// Copyright 2024 AlphDevCode. All Rights Reserved.


#include "AbilityHealSelf.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void AAbilityHealSelf::Initialize()
{
	Super::Initialize();

	//TODO: Move this binding to Ability parent class
	OnAbilityActivated.AddDynamic(this, &AAbilityHealSelf::AbilityActivated);
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

	if(HealFX == nullptr)
	{
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red,
				TEXT("HealFX is not valid!"));
		return;
	}
	UGameplayStatics::SpawnEmitterAttached(HealFX, Caster->GetMesh(), "Chest");

	SpawnAbilityEffects();
}


