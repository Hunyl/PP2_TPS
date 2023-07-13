// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter_PracticeTarget.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"

AEnemyCharacter_PracticeTarget::AEnemyCharacter_PracticeTarget()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh_Main = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh_Main"));
	Collision_Main = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision_Main"));

	Mesh_Main->SetupAttachment(RootComponent);
	Collision_Main->SetupAttachment(Mesh_Main);

	ActivatedRotation = FRotator(-90.0f, 0.0f, 0.0f);
	DeactivatedRotation = FRotator(0.0f, 0.0f, 0.0f);

	ActivationLerpValue = 0.0f;
	IsActive = false;

	GetMesh()->SetEnableGravity(false);
	GetCapsuleComponent()->SetEnableGravity(false);
	GetCapsuleComponent()->InitCapsuleSize(0.0f, 0.0f);
}

void AEnemyCharacter_PracticeTarget::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyCharacter_PracticeTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsActive)
	{
		ActivationLerpValue += DeltaTime * 10.0f;
	}
	else
	{
		ActivationLerpValue -= DeltaTime * 4.5f;
	}

	ActivationLerpValue = FMath::Clamp(ActivationLerpValue, 0.0f, 1.0f);
	MedianRotation = FMath::Lerp(DeactivatedRotation, ActivatedRotation, ActivationLerpValue);
	FQuat MedianQuat = FQuat(MedianRotation);

	Mesh_Main->SetRelativeRotation(MedianQuat);
}

void AEnemyCharacter_PracticeTarget::TakeDamage(float Damage)
{
	Super::TakeDamage(Damage);
}

void AEnemyCharacter_PracticeTarget::OnDefeated()
{
	Super::OnDefeated();

	IsActive = true;

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemyCharacter_PracticeTarget::OnRespawned, 2.5f, false);
}

void AEnemyCharacter_PracticeTarget::OnRespawned()
{
	Super::OnRespawned();

	IsActive = false;
	HitPoint = HitPoint_Max;
}
