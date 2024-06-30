// Copyright 2024 AlphDevCode. All Rights Reserved.


#include "Actors/Abilities/AbilityEffectAOE.h"

#include "Actors/Abilities/Ability.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


AAbilityEffectAOE::AAbilityEffectAOE()
{
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);
}

void AAbilityEffectAOE::OnActivate()
{
	Super::OnActivate();

	TArray<AActor*> OverlappingActors;
	SphereCollision->GetOverlappingActors(OverlappingActors, ClassFilter);

	for (AActor* Actor : OverlappingActors)
	{
		if(ParentAbility->Caster != Actor)
		{
			UGameplayStatics::ApplyDamage(Actor, BaseDamage, ParentAbility->Caster->GetController(),
				GetOwner(), UDamageType::StaticClass());
		}
	}

	Destroy();
}
