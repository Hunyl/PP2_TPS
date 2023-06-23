// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "EnemyCharacter_PracticeTarget.generated.h"

/**
 * 
 */

class UStaticMeshComponent;

UCLASS()
class PP2_TPS_API AEnemyCharacter_PracticeTarget : public AEnemyCharacter
{
	GENERATED_BODY()

public:

	AEnemyCharacter_PracticeTarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(EditAnywhere, Category = "Basic Component")
	UStaticMeshComponent* Mesh_Main;

public:

	virtual void TakeDamage(float Damage) override;

	virtual void OnDefeated() override;

	virtual void OnRespawned() override;

private:

	FRotator ActivatedRotation;

	FRotator DeactivatedRotation;

	FRotator MedianRotation;

	float ActivationLerpValue;

	bool IsActive;

};
