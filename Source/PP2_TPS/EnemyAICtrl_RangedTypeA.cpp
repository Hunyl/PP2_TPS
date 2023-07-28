// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAICtrl_RangedTypeA.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#include "Perception/AIPerceptionTypes.h"

#include "PlayerCharacter.h"
#include "EnemyCharacter.h"

const FName AEnemyAICtrl_RangedTypeA::PlayerCharacter(TEXT("PlayerCharacter"));
const FName AEnemyAICtrl_RangedTypeA::PlayerVisible(TEXT("IsPlayerVisible"));
const FName AEnemyAICtrl_RangedTypeA::AttackAvailable(TEXT("IsAttackAvailable"));

AEnemyAICtrl_RangedTypeA::AEnemyAICtrl_RangedTypeA()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> Object_BehaviorTree(TEXT("/Game/AIs/AI_RangedTypeA/BT_RangedTypeA.BT_RangedTypeA"));
	if (Object_BehaviorTree.Succeeded())
	{
		Asset_BehaviorTree = Object_BehaviorTree.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> Object_BlackBoard(TEXT("/Game/AIs/AI_RangedTypeA/BB_RangedTypeA.BB_RangedTypeA"));
	if (Object_BlackBoard.Succeeded())
	{
		Asset_BlackBoard = Object_BlackBoard.Object;
	}

	InitializePerception();

}

void AEnemyAICtrl_RangedTypeA::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OwnerEnemy = Cast<AEnemyCharacter>(InPawn);

	if (!IsValid(Asset_BehaviorTree) || !IsValid(Asset_BlackBoard))
	{
		return;
	}

	if (UseBlackboard(Asset_BlackBoard, BlackBoardComp))
	{
		if (!RunBehaviorTree(Asset_BehaviorTree))
		{
			UE_LOG(LogTemp, Log, TEXT("Unable To Run Behavior Tree"));
		}
	}
}

void AEnemyAICtrl_RangedTypeA::OnUnPossess()
{
	Super::OnUnPossess();
}

void AEnemyAICtrl_RangedTypeA::OnPlayerVisible(AActor* Actor, FAIStimulus Stimulus)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(Actor);
	if (!IsValid(Player))
	{
		return;
	}

	Blackboard->SetValueAsObject(PlayerCharacter, Player);

	Blackboard->SetValueAsBool(PlayerVisible, Stimulus.WasSuccessfullySensed());

	if (!Stimulus.WasSuccessfullySensed())
	{
		return;
	}

	if (!OwnerEnemy.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Unable to Get Owner"));
		return;
	}

	FVector OwnerLocation = OwnerEnemy->GetActorLocation();
	
	FVector PlayerLocation = Player->GetActorLocation();

	float Distance = FVector::Distance(OwnerLocation, PlayerLocation);

	bool IsInAttackRange = Distance <= OwnerEnemy->GetAttackRange() ? true : false;

	Blackboard->SetValueAsBool(AttackAvailable, IsInAttackRange);
}

void AEnemyAICtrl_RangedTypeA::InitializePerception()
{
	AIPerception = CreateOptionalDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);

	AIConfig_Sight = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("AIConfig_Sight"));
	AIConfig_Sight->SightRadius = 1000.0f;
	AIConfig_Sight->LoseSightRadius = AIConfig_Sight->SightRadius + 100.0f;
	AIConfig_Sight->PeripheralVisionAngleDegrees = 75.0f;

	FAISenseAffiliationFilter SenseFilter;
	SenseFilter.bDetectEnemies = true;
	SenseFilter.bDetectFriendlies = true;
	SenseFilter.bDetectNeutrals = true;
	AIConfig_Sight->DetectionByAffiliation = SenseFilter;

	if (!IsValid(GetPerceptionComponent()))
	{
		return;
	}

	GetPerceptionComponent()->SetDominantSense(AIConfig_Sight->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAICtrl_RangedTypeA::OnPlayerVisible);
	GetPerceptionComponent()->ConfigureSense(*AIConfig_Sight);
}
