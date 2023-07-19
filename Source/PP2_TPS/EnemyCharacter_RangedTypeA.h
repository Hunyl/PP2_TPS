// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "EnemyCharacter_RangedTypeA.generated.h"

/**
 * 
 */

class UPawnSensingComponent;
class UBlackboardComponent;

UCLASS()
class PP2_TPS_API AEnemyCharacter_RangedTypeA : public AEnemyCharacter
{
	GENERATED_BODY()
	
public:

	AEnemyCharacter_RangedTypeA();

protected:
	
	virtual void BeginPlay() override;

public:
	
	virtual void Tick(float DeltaTime) override;

public:
	// AI Components
	UPROPERTY(EditAnywhere, Category = "AI")
	UPawnSensingComponent* PawnSensing;

public:
	// BlackBoard Keys
	static const FName PlayerVisible;

public:
	// Unreal Functions
	UFUNCTION()
	void OnPlayerVisible(APawn* Player);

	UFUNCTION()
	void OnPlayerInvisible();

	//Methods
	UBlackboardComponent* GetBlackBoard();

};
