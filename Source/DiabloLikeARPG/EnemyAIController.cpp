// Copyright 2024 AlphDevCode. All Rights Reserved.


#include "EnemyAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/DiabloLikeARPGCharacter.h"
#include "Components/AbilitiesComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Sight.h"


AEnemyAIController::AEnemyAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	AIPerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(
		TEXT("AIPerceptionStimuliSourceComponent"));
	AIPerceptionStimuliSourceComponent->RegisterForSense(UAISense_Sight::StaticClass());
	AIPerceptionStimuliSourceComponent->RegisterForSense(UAISense_Damage::StaticClass());
	AIPerceptionStimuliSourceComponent->bAutoRegister = true;

	bShouldLookForPlayer = true;
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimerForNextTick(this, &AEnemyAIController::Initialize);

	AIPerceptionComponent->OnTargetPerceptionUpdated.
	                       AddDynamic(this, &AEnemyAIController::OnTargetPerceptionUpdated);
}

void AEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	const ADiabloLikeARPGCharacter* PerceivedCharacter = Cast<ADiabloLikeARPGCharacter>(Actor);
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (PerceivedCharacter && PerceivedCharacter->GetController() == PlayerController && Stimulus.
		WasSuccessfullySensed())
	{
		const TSubclassOf<UAISense> SenseClass =
			UAIPerceptionSystem::GetSenseClassForStimulus(this, Stimulus);
		if (SenseClass == UAISense_Sight::StaticClass() || SenseClass == UAISense_Damage::StaticClass())
		{
			SetAttackTarget(Actor);
		}
	}
}

void AEnemyAIController::Initialize()
{
	ControlledCharacter = Cast<ADiabloLikeARPGCharacter>(GetCharacter());

	if (!ControlledCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("Enemy AI Controller: Controlled character is null for %s"), *GetName());
	}

	if (AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);
	}

	if (Blackboard)
	{
		if (ControlledCharacter != nullptr)
			Blackboard->SetValueAsFloat(IdealRangeKey, GetIdealRange());
	}

	if (bAutoChasePlayer)
	{
		SetAttackTarget(UGameplayStatics::GetPlayerCharacter(this, 0));
	}
}

void AEnemyAIController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	if (Blackboard != nullptr)
	{
		Blackboard->ClearValue(TargetEnemyKey);
	}
	bShouldLookForPlayer = false;
}

AActor* AEnemyAIController::GetAttackTarget() const
{
	return Cast<AActor>(Blackboard->GetValueAsObject(TargetEnemyKey));
}

float AEnemyAIController::GetIdealRange() const
{
	if (ControlledCharacter == nullptr)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
			"Can not get ideal range. Controlled character is null! Returning default value.");
		}
		return 150.f;
	}
	return ControlledCharacter->GetAbilitiesComponent()->GetCurrentAbilityRange();
}

void AEnemyAIController::SetAttackTarget(AActor* NewAttackTarget)
{
	if (Blackboard != nullptr && bShouldLookForPlayer)
	{
		Blackboard->SetValueAsObject(TargetEnemyKey, NewAttackTarget);
	}
}

void AEnemyAIController::SetAutoChasePlayer(const bool NewAutoChasePlayer)
{
	bAutoChasePlayer = NewAutoChasePlayer;
}
