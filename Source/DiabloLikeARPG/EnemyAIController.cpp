// Copyright 2024 AlphDevCode. All Rights Reserved.


#include "EnemyAIController.h"

#include "AbilitiesSystem/AbilitiesComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/DiabloLikeARPGCharacter.h"
#include "GameFramework/GameModeBase.h"
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
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	ControlledCharacter = Cast<ADiabloLikeARPGCharacter>(GetCharacter());
	bShouldLookForPlayer = true;

	if (AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);
	}

	if (Blackboard)
	{
		if (ControlledCharacter != nullptr)
			Blackboard->SetValueAsFloat(IdealRangeKey, GetIdealRange());
	}

	AIPerceptionComponent->OnTargetPerceptionUpdated
		.AddDynamic(this, &AEnemyAIController::OnTargetPerceptionUpdated);
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
			if (Blackboard != nullptr && bShouldLookForPlayer)
			{
				Blackboard->SetValueAsObject(TargetEnemyKey, Actor);
			}
		}
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
	return ControlledCharacter->AbilitiesComponent->GetCurrentAbilityRange();
}
