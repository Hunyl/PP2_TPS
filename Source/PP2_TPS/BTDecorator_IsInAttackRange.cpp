// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsInAttackRange.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "PlayerCharacter.h"
#include "EnemyAICtrl_RangedTypeA.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("IsInAttackRange");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool Result = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* CtrlPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!IsValid(CtrlPawn))
	{
		return false;
	}

	APlayerCharacter* Player = Cast<APlayerCharacter>(OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsObject(AEnemyAICtrl_RangedTypeA::PlayerCharacter));
	if (!IsValid(Player))
	{
		return false;
	}

	Result = (Player->GetDistanceTo(CtrlPawn) <= 500.0f);

	return Result;
}
