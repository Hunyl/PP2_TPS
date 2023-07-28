// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "DamageableInterface.h"

#include "EnemyCharacter.generated.h"

UCLASS()
class PP2_TPS_API AEnemyCharacter : public ACharacter, public IDamageableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	UPROPERTY(EditAnywhere, Category = "Basic Status")
	float HitPoint_Max;

protected:
	
	UPROPERTY(EditAnywhere, Category = "Basic Status")
	float AttackRange;

	float HitPoint;

public:

	virtual void TakeDamage(float Damage) override;

	virtual void OnDefeated();

	virtual void OnRespawned();

	float GetAttackRange();

};
