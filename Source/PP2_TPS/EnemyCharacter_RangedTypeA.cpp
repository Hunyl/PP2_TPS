// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter_RangedTypeA.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

#include "EnemyAICtrl_RangedTypeA.h"

const FName AEnemyCharacter_RangedTypeA::PlayerVisible(TEXT("IsPlayerVisible"));

AEnemyCharacter_RangedTypeA::AEnemyCharacter_RangedTypeA()
{
	PrimaryActorTick.bCanEverTick = true;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));

	AIControllerClass = AEnemyAICtrl_RangedTypeA::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AEnemyCharacter_RangedTypeA::BeginPlay()
{
	Super::BeginPlay();

	PawnSensing->OnSeePawn.AddDynamic(this, &AEnemyCharacter_RangedTypeA::OnPlayerVisible);
}

void AEnemyCharacter_RangedTypeA::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

UBlackboardComponent* AEnemyCharacter_RangedTypeA::GetBlackBoard()
{
	if (!IsValid(GetController()))
	{
		return nullptr;
	}

	AAIController* AICtrl = Cast<AAIController>(GetController());
	if (!IsValid(AICtrl))
	{
		return nullptr;
	}

	UBlackboardComponent* BlackBoard = AICtrl->GetBlackboardComponent();
	if (!IsValid(BlackBoard))
	{
		return nullptr;
	}

	return BlackBoard;
}

void AEnemyCharacter_RangedTypeA::OnPlayerVisible(APawn* Player)
{
	TWeakObjectPtr<UBlackboardComponent> BlackBoard = AEnemyCharacter_RangedTypeA::GetBlackBoard();
	if (!BlackBoard.IsValid())
	{
		return;
	}

	BlackBoard->SetValueAsBool(PlayerVisible, true);

	UE_LOG(LogTemp, Log, TEXT("Player Visible"));
}

void AEnemyCharacter_RangedTypeA::OnPlayerInvisible()
{
	TWeakObjectPtr<UBlackboardComponent> BlackBoard = AEnemyCharacter_RangedTypeA::GetBlackBoard();
	if (!BlackBoard.IsValid())
	{
		return;
	}

	BlackBoard->SetValueAsBool(PlayerVisible, false);
}
