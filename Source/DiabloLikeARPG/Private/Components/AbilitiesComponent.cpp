// Copyright 2024 AlphDevCode. All Rights Reserved.


#include "Components/AbilitiesComponent.h"

#include "Actors/Abilities/Ability.h"
#include "Actors/Characters/DiabloLikeARPGCharacter.h"
#include "Actors/Characters/EnemyARPGCharacter.h"
#include "Actors/Characters/PlayerARPGCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Logger.h"

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
		AAbility* Ability = GetWorld()->SpawnActorDeferred<AAbility>(AbilityClass, FTransform::Identity, GetOwner());
		// Ability->SetOwner(GetOwner());
		Ability->Caster = Cast<ACharacter>(GetOwner());

		// If the owner is an enemy set the target of all abilities to the player
		if (Cast<AEnemyARPGCharacter>(GetOwner()))
		{
			Ability->Target = UGameplayStatics::GetPlayerPawn(this, 0);
		}

		 // LOG_WARNING(TEXT("Owner of ability is: %s"), *GetOwner()->GetName());

		Ability->FinishSpawning(FTransform::Identity);
		AbilitiesArray.Add(Ability);
	}
	else
	{
		AbilitiesArray.Add(nullptr);
	}
}

void UAbilitiesComponent::ActivatePrimaryAttackAbility()
{
	if (ActivateClickAbility(0) == nullptr) return;

	// Only assign the Target to the last interactable if we are a player
	if (ADiabloLikeARPGCharacter* OwnerCharacter = Cast<APlayerARPGCharacter>(GetOwner()))
	{
		ClickAssignedAbilities[0]->Target = OwnerCharacter->GetTargetInteractableActor();
	}
}

float UAbilitiesComponent::GetCurrentAbilityRange() const
{
	if (LastActivatedAbility != nullptr)
	{
		return LastActivatedAbility->GetRange();
	}
	if (ClickAssignedAbilities.IsValidIndex(0) && ClickAssignedAbilities[0] != nullptr)
	{
		return ClickAssignedAbilities[0]->GetRange();
	}
	if (AssignedAbilities.IsValidIndex(0) && AssignedAbilities[0] != nullptr)
	{
		return AssignedAbilities[0]->GetRange();
	}
	return 150.f;
}

const AAbility* UAbilitiesComponent::ActivateAbility(const int AbilityIndex)
{
	return ActivateAbilityFromCollection(AssignedAbilities, AbilityIndex);
}

const AAbility* UAbilitiesComponent::ActivateClickAbility(const int AbilityIndex)
{
	return ActivateAbilityFromCollection(ClickAssignedAbilities, AbilityIndex);
}

void UAbilitiesComponent::CleanAbilities()
{
	for (AAbility* Ability : ClickAssignedAbilities)
	{
		if (Ability != nullptr)
		{
			Ability->Destroy();
		}
	}

	for (AAbility* Ability : AssignedAbilities)
	{
		if (Ability != nullptr)
		{
			Ability->Destroy();
		}
	}
}

const AAbility* UAbilitiesComponent::ActivateAbilityFromCollection(
	const TArray<AAbility*>& AbilitiesArray, const int AbilityIndex)
{
	if (!AbilitiesArray.IsValidIndex(AbilityIndex) || AbilitiesArray[AbilityIndex] == nullptr)
	{
		LOG_WARNING(TEXT("No ability assigned to selected Ability spot"));
		return nullptr;
	}

	if (LastActivatedAbility != nullptr)
	{
		// If the last activated ability is still active, we can not activate another ability
		// We ignore this restriction for the primary attack ability
		if(LastActivatedAbility->GetIsActive() && LastActivatedAbility != ClickAssignedAbilities[0])
		{
			LOG_WARNING(TEXT("Can not activate ability [%s]. Another ability [%s] is already active."),
						*AbilitiesArray[AbilityIndex]->GetAbilityName().ToString(),
						*LastActivatedAbility->GetAbilityName().ToString());
			return nullptr;
		}
		if (LastActivatedAbility != AbilitiesArray[AbilityIndex])
		{
			LastActivatedAbility->ResetAbility();
		}
	}
	AbilitiesArray[AbilityIndex]->ActivateAbility(GetOwner()->GetActorLocation());
	LastActivatedAbility = AbilitiesArray[AbilityIndex];
	return AbilitiesArray[AbilityIndex];
}
