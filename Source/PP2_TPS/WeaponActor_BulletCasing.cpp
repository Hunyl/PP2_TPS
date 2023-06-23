// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActor_BulletCasing.h"

#include "Components/StaticMeshComponent.h"

// Sets default values
AWeaponActor_BulletCasing::AWeaponActor_BulletCasing()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	Mesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AWeaponActor_BulletCasing::BeginPlay()
{
	Super::BeginPlay();
	
	Mesh->AddImpulseAtLocation(GetActorForwardVector() * -10.0f, GetActorLocation());

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AWeaponActor_BulletCasing::DestroyActor, 2.0f, false);
}

// Called every frame
void AWeaponActor_BulletCasing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponActor_BulletCasing::DestroyActor()
{
	Destroy();
}

