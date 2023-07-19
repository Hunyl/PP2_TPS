// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAICtrl_RangedTypeA.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

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
}

void AEnemyAICtrl_RangedTypeA::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

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
