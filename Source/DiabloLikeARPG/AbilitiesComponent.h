// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilitiesComponent.generated.h"

class AAbility;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIABLOLIKEARPG_API UAbilitiesComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAbilitiesComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<AAbility>> AssignedAbilityClasses;

	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<AAbility>> ClickAssignedAbilitiesClasses;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TArray<AAbility*> AssignedAbilities;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TArray<AAbility*> ClickAssignedAbilities;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void AddAbility(const TSubclassOf<AAbility>& AbilityClass,
		TArray<AAbility*>& AbilitiesArray) const;
	
	void ActivateAttackAbility() const;
};
