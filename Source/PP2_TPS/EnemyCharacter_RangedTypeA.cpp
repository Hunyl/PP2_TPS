// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter_RangedTypeA.h"

#include "Perception/PawnSensingComponent.h"

AEnemyCharacter_RangedTypeA::AEnemyCharacter_RangedTypeA()
{
	PrimaryActorTick.bCanEverTick = true;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
}

void AEnemyCharacter_RangedTypeA::BeginPlay()
{
	Super::BeginPlay();


}

void AEnemyCharacter_RangedTypeA::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}