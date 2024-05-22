// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitiesComponent.h"

#include "AbilityAttack.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UAbilitiesComponent::UAbilitiesComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
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


// Called every frame
void UAbilitiesComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAbilitiesComponent::AddAbility(const TSubclassOf<AAbility>& AbilityClass, TArray<AAbility*>& AbilitiesArray) const
{
	if (AbilityClass != nullptr)
	{
		AAbility* Ability = GetWorld()->SpawnActor<AAbility>(AbilityClass);
		Ability->SetOwner(GetOwner());
		Ability->Caster = Cast<ACharacter>(GetOwner());
		Ability->Target = Cast<ACharacter>(GetOwner());
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
	
	ClickAssignedAbilities[0]->ActivateAbility(); 
}
