// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter_RangedTypeA.h"

#include "AIController.h"

#include "EnemyAICtrl_RangedTypeA.h"

AEnemyCharacter_RangedTypeA::AEnemyCharacter_RangedTypeA()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AEnemyAICtrl_RangedTypeA::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AEnemyCharacter_RangedTypeA::BeginPlay()
{
	Super::BeginPlay();

	
}

void AEnemyCharacter_RangedTypeA::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

