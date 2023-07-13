// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	HitPoint = HitPoint_Max;
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::TakeDamage(float Damage)
{
	HitPoint -= Damage;

	UE_LOG(LogTemp, Log, TEXT("%s took damage with %f point"), *FString(this->GetName()), Damage);

	if (HitPoint <= 0.0f)
	{
		HitPoint = -1.0f;
		OnDefeated();
	}
}

void AEnemyCharacter::OnDefeated()
{
	UE_LOG(LogTemp, Log, TEXT("Defeated: %s"), *FString(this->GetName()));
}

void AEnemyCharacter::OnRespawned()
{
	UE_LOG(LogTemp, Log, TEXT("Respawned or Reactivated: %s"), *FString(this->GetName()));
}

