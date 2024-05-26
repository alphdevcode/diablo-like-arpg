// Copyright 2024 AlphDevCode. All Rights Reserved.


#include "AbilitiesComponent.h"

#include "AbilityAttack.h"
#include "DiabloLikeARPG/Characters/DiabloLikeARPGCharacter.h"
#include "DiabloLikeARPG/Characters/EnemyARPGCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

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

		// If the owner is an enemy set the target of all abilities to the player
		if(Cast<AEnemyARPGCharacter>(GetOwner()))
		{
			Ability->Target = UGameplayStatics::GetPlayerPawn(this, 0);
		}

		AbilitiesArray.Add(Ability);
	}
	else
	{
		AbilitiesArray.Add(nullptr);
	}
}

void UAbilitiesComponent::ActivatePrimaryAttackAbility()
{
	if(ActivateClickAbility(0) == nullptr) return;

	ADiabloLikeARPGCharacter* OwnerCharacter = Cast<ADiabloLikeARPGCharacter>(GetOwner());

	// Only assign the Target to the last interactable if we are the player
	if(OwnerCharacter != nullptr
		&& OwnerCharacter == UGameplayStatics::GetPlayerCharacter(this,0))
	{
		ClickAssignedAbilities[0]->Target = OwnerCharacter->GetTargetInteractableActor();
	}
}

const AAbility* UAbilitiesComponent::ActivateAbility(const int AbilityIndex)
{
    return ActivateAbilityFromCollection(AssignedAbilities, AbilityIndex);
}

const AAbility* UAbilitiesComponent::ActivateClickAbility(const int AbilityIndex)
{
    return ActivateAbilityFromCollection(ClickAssignedAbilities, AbilityIndex);
}

const AAbility* UAbilitiesComponent::ActivateAbilityFromCollection(
	const TArray<AAbility*>& AbilitiesArray, const int AbilityIndex)
{
    if (!AbilitiesArray.IsValidIndex(AbilityIndex) || AbilitiesArray[AbilityIndex] == nullptr)
    {
        if(GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow,
            TEXT("No ability assigned to selected Ability spot"));
        return nullptr;
    }
    
    AbilitiesArray[AbilityIndex]->ActivateAbility(GetOwner()->GetActorLocation());
    return AbilitiesArray[AbilityIndex];
}
