// Copyright 2024 AlphDevCode. All Rights Reserved.


#include "AbilityWithSingleAnimationBase.h"

#include "GameFramework/Character.h"

void AAbilityWithSingleAnimationBase::AbilityActivated()
{
	Super::AbilityActivated();

	if(AbilityAnimMontage == nullptr)
	{
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red,
				TEXT("AbilityAnimMontage is not valid!"));
		return;
	}
	Caster->PlayAnimMontage(AbilityAnimMontage);

}
