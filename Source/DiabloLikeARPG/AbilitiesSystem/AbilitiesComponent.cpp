// Copyright 2024 AlphDevCode. All Rights Reserved.


#include "AbilitiesComponent.h"

#include "AbilityAttack.h"
#include "DiabloLikeARPG/Characters/DiabloLikeARPGCharacter.h"
#include "GameFramework/Character.h"

UAbilitiesComponent::UAbilitiesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAbilitiesComponent::BeginPlay()
{
	Super::BeginPlay();

	for (const TSubclassOf<AAbility>& AbilityClass : AssignedAbilityClasses)
	{
		AddAbility(AbilityClass, AssignedAbilities);
	}

	for (const TSubclassOf<AAbility>& AbilityClass : ClickAssignedAbilitiesClasses)
	{
		AddAbility(AbilityClass, ClickAssignedAbilities);
	}
}


void UAbilitiesComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAbilitiesComponent::AddAbility(const TSubclassOf<AAbility>& AbilityClass, TArray<AAbility*>& AbilitiesArray) const
{
	if (AbilityClass != nullptr)
	{
		AAbility* Ability = GetWorld()->SpawnActor<AAbility>(AbilityClass);
		Ability->SetOwner(GetOwner());
		Ability->Caster = Cast<ACharacter>(GetOwner());
		AbilitiesArray.Add(Ability);
	}
	else
	{
		AbilitiesArray.Add(nullptr);
	}
}

void UAbilitiesComponent::ActivatePrimaryAttackAbility() const
{
	if (ClickAssignedAbilities[0] == nullptr)
	{
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow,
											 "No ability assigned to Primary Ability spot");
		return;
	}
	
	ClickAssignedAbilities[0]->ActivateAbility(GetOwner()->GetActorLocation());

	ADiabloLikeARPGCharacter* OwnerCharacter = Cast<ADiabloLikeARPGCharacter>(GetOwner());
	if(OwnerCharacter != nullptr)
	{
		ClickAssignedAbilities[0]->Target = OwnerCharacter->GetLastInteractableActor();
	}
}
