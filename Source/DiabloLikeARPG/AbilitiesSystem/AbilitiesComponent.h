// Copyright 2024 AlphDevCode. All Rights Reserved.

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
	UAbilitiesComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<AAbility>> AssignedAbilityClasses;

	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<AAbility>> ClickAssignedAbilitiesClasses;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TArray<AAbility*> AssignedAbilities;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TArray<AAbility*> ClickAssignedAbilities;

	UPROPERTY()
	AAbility* LastActiveAbility;

private:
	const AAbility* ActivateAbilityFromCollection(
		const TArray<AAbility*>& AbilitiesArray,
		const int AbilityIndex);

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void AddAbility(const TSubclassOf<AAbility>& AbilityClass,
		TArray<AAbility*>& AbilitiesArray) const;

	UFUNCTION(BlueprintCallable)
	void ActivatePrimaryAttackAbility();

	const AAbility* ActivateAbility(const int AbilityIndex);
	const AAbility* ActivateClickAbility(const int AbilityIndex);
};
