// Copyright 2024 AlphDevCode. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class UProjectileMovementComponent;
class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProjectileHit, AActor*, OtherActor, const FHitResult&, Hit);

UCLASS()
class DIABLOLIKEARPG_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectileBase();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* ArrowComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovementComponent;	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* ProjectileParticleSystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Visual", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ImpactFX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* ImpactSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileSettings", meta = (AllowPrivateAccess = "true"))
	float InitialSpeed = 500.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileSettings", meta = (AllowPrivateAccess = "true"))
	float MaxSpeed = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileSettings", meta = (AllowPrivateAccess = "true"))
	float GravityScale = 500.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileSettings", meta = (AllowPrivateAccess = "true"))
	float Damage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targetting", meta = (AllowPrivateAccess = "true"))
	AActor* Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileSettings", meta = (AllowPrivateAccess = "true"))
	bool bIsHomingTarget = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileSettings", meta = (AllowPrivateAccess = "true", EditCondition = "bIsHomingTarget == true"))
	float HomingAccelerationMagnitude = 2000.f;

private:
	void RotateToTarget();

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetDamage(float NewDamage);

	UPROPERTY(BlueprintAssignable)
	FOnProjectileHit OnProjectileHit;
};
