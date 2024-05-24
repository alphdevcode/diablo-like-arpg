#pragma once

#include "CoreMinimal.h"
#include "DiabloLikeARPG/InteractionSystem/InteractorInterface.h"
#include "GameFramework/Character.h"
#include "DiabloLikeARPGCharacter.generated.h"

class UNiagaraSystem;
class AAbility;
class AAbilityAttack;
class IInteractableInterface;

UCLASS(Blueprintable)
class ADiabloLikeARPGCharacter : public ACharacter, public IInteractorInterface
{
	GENERATED_BODY()

public:
	ADiabloLikeARPGCharacter();

	virtual void BeginPlay() override;

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;
	
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent,
	                         AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UPROPERTY(VisibleAnywhere)
	class UStatsComponent* StatsComponent;

	UPROPERTY(VisibleAnywhere)
	class UAbilitiesComponent* AbilitiesComponent;

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	FTimerHandle InteractionTimerHandle;
	FTimerHandle DestroyActorTimerHandle;

protected:
	IInteractableInterface* CurrentInteractable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* HitAnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* DestroyFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USoundBase* DestroySound;

	void CheckForInteractions();

	void DestroyCharacter();

public:
	virtual void SetInteractableTarget(IInteractableInterface* Interactable) override;
	virtual float GetInteractionRange() override;

	void ActivatePrimaryAttackAbility() const;
};
