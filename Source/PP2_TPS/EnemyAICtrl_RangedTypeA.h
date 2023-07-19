// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAICtrl_RangedTypeA.generated.h"

/**
 * 
 */

class UBehaviorTree;
class UBlackboardData;
class UBlackboardComponent;

UCLASS()
class PP2_TPS_API AEnemyAICtrl_RangedTypeA : public AAIController
{
	GENERATED_BODY()

public:

	AEnemyAICtrl_RangedTypeA();
	
public:

	virtual void OnPossess(APawn* InPawn) override;
	
	virtual void OnUnPossess() override;

public:

	UPROPERTY()
	UBehaviorTree* Asset_BehaviorTree;

	UPROPERTY()
	UBlackboardData* Asset_BlackBoard;

	UPROPERTY()
	UBlackboardComponent* BlackBoardComp;
};
