// Copyright 2024 AlphDevCode. All Rights Reserved.


#include "Actors/ProjectileBase.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Root Component"));
	RootComponent = BoxComponent;
	
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	
	ProjectileParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Projectile Particle System"));
	ProjectileParticleSystemComponent->SetupAttachment(RootComponent);
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement Component");
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>("Arrow Component");
	ArrowComponent->SetupAttachment(RootComponent);

	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->InitialSpeed = InitialSpeed;
	ProjectileMovementComponent->MaxSpeed = MaxSpeed;
	ProjectileMovementComponent->ProjectileGravityScale = GravityScale;
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	if(BoxComponent != nullptr)
	{
		BoxComponent->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
	}

	if(bIsHomingTarget && Target != nullptr)
	{
		ProjectileMovementComponent->bIsHomingProjectile = true;
		ProjectileMovementComponent->HomingTargetComponent = Target->GetRootComponent();
		ProjectileMovementComponent->HomingAccelerationMagnitude = HomingAccelerationMagnitude;
	}

	RotateToTarget();
	IgnoreCollisionsWithCaster();
}

void AProjectileBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if(BoxComponent != nullptr)
	{
		BoxComponent->OnComponentHit.RemoveDynamic(this, &AProjectileBase::OnHit);
	}
}

void AProjectileBase::RotateToTarget()
{
	if(Target != nullptr)
	{
		ProjectileMovementComponent->Velocity = UKismetMathLibrary::GetDirectionUnitVector(
			GetActorLocation(), Target->GetActorLocation()) * InitialSpeed;
	}
}

void AProjectileBase::IgnoreCollisionsWithCaster()
{
	BoxComponent->IgnoreActorWhenMoving(GetOwner(), true);
	if(UShapeComponent* ShapeComponent = GetOwner()->FindComponentByClass<UShapeComponent>();
		ShapeComponent != nullptr)
	{
		ShapeComponent->IgnoreActorWhenMoving(this, true);
	} 
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                            FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor == GetOwner())
	{
		return;
	}
	
	OnProjectileHit.Broadcast(OtherActor, Hit);

	if(ImpactFX != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactFX, Hit.Location,
			GetActorRotation(), FVector(1), true, EPSCPoolMethod::AutoRelease);
	}

	if(ImpactSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, Hit.ImpactPoint);
	}

	UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(),
		this, nullptr);
	
	Destroy();
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileBase::SetDamage(const float NewDamage)
{
	Damage = NewDamage;

}

void AProjectileBase::SetTarget(AActor* TargetActor)
{
	Target = TargetActor;
}

void AProjectileBase::InitializeValues(const float NewInitialSpeed, const float NewMaxSpeed, const float NewGravityScale,
                                       const bool bNewIsHomingTarget, const float NewHomingAccelerationMagnitude)
{
	InitialSpeed = NewInitialSpeed;
	MaxSpeed = NewMaxSpeed;
	GravityScale = NewGravityScale;
	bIsHomingTarget = bNewIsHomingTarget;
	HomingAccelerationMagnitude = NewHomingAccelerationMagnitude;
}


