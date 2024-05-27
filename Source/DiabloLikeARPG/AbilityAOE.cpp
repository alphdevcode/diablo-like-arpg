// Copyright 2024 AlphDevCode. All Rights Reserved.


#include "AbilityAOE.h"

#include "DiabloLikeARPGAnimInstance.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void AAbilityAOE::Initialize()
{
	Super::Initialize();

	OnAbilityActivated.AddDynamic(this, &AAbilityAOE::AbilityActivated);

}

void AAbilityAOE::AbilityActivated()
{
	if(AbilityAnimMontage == nullptr)
	{
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red,
				TEXT("AbilityAnimMontage is not valid!"));
		return;
	}
	Caster->PlayAnimMontage(AbilityAnimMontage);

	CasterAnimInstance->OnAnimNotifyAttackPeak
		.AddDynamic(this, &AAbilityAOE::HandleAbilityEffectsSpawning);
}

void AAbilityAOE::HandleAbilityEffectsSpawning()
{
	if(AController* CasterController = Caster->GetController(); CasterController != nullptr)
	{
		CasterController->StopMovement();
	}
	SpawnAbilityEffectsWithLocation(Caster->GetMesh()->GetSocketLocation(AbilityEffectsSocket));
	CasterAnimInstance->OnAnimNotifyAttackPeak
		.RemoveDynamic(this, &AAbilityAOE::HandleAbilityEffectsSpawning);
}
