// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActor_Gun.h"

#include "Components/SkeletalMeshComponent.h"
#include "../../../../Engine/Plugins/FX/Niagara/Source/Niagara/Classes/NiagaraSystem.h"
#include "../../../../Engine/Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h"
#include "../../../../Engine/Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"

#include "EnemyCharacter.h"

AWeaponActor_Gun::AWeaponActor_Gun()
{
	PrimaryActorTick.bCanEverTick = true;

	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));

	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));

	Niagara->bAutoActivate = false;
}

void AWeaponActor_Gun::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(Mesh) && IsValid(Muzzle))
	{
		Muzzle->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("MuzzleFlash"));
	}

	if (IsValid(Niagara))
	{
		Niagara->AttachToComponent(Muzzle, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}

	CalcRPMToDelay();

	switch (FireModeType)
	{
		case EFireModeType::Semi:
			FireDelaySequence.BindUFunction(this, FName("CalcFireDelay_Semi"));
			break;
		case EFireModeType::Auto:
			FireDelaySequence.BindUFunction(this, FName("CalcFireDelay_Auto"));
			break;
		default:
			break;
	}

	switch (WeaponType)
	{
		case EWeaponType::Pistol:
			FireSequence.BindUFunction(this, FName("FireWeapon_Pistol"));
			break;
		case EWeaponType::Rifle:
			FireSequence.BindUFunction(this, FName("FireWeapon_Rifle"));
			break;
		case EWeaponType::Shotgun:
			FireSequence.BindUFunction(this, FName("FireWeapon_Shotgun"));
			break;
		default:
			break;
	}

	IsReadyToFire = true;
}

void AWeaponActor_Gun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FireDelayCounter -= DeltaTime;

	if (FireDelayCounter <= 0.0f)
	{
		FireDelayCounter = -1.0f;
	}
}

void AWeaponActor_Gun::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	Super::OnComponentBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, Hit);

	UE_LOG(LogTemp, Log, TEXT("Picked Up a Gun"));
}

void AWeaponActor_Gun::Attack(bool IsHolding)
{
	Super::Attack(IsHolding);

	if (!FireDelaySequence.IsBound())
	{
		return;
	}

	if (FireDelaySequence.Execute(IsHolding))
	{
		FireSequence.ExecuteIfBound();
	}
}

bool AWeaponActor_Gun::IsOnFireDelay()
{
	if (FireDelayCounter > 0.0f)
	{
		return true;
	}

	return false;
}

void AWeaponActor_Gun::Fire()
{
	UE_LOG(LogTemp, Log, TEXT("Fire"));
}

void AWeaponActor_Gun::FireWeapon_Pistol()
{
	UE_LOG(LogTemp, Log, TEXT("Fire Pistol"));
}

void AWeaponActor_Gun::FireWeapon_Rifle()
{
	FHitResult Hit;

	FVector StartVector = IsValid(Muzzle) ? Muzzle->GetComponentLocation() : FVector(0.0f, 0.0f, 0.0f);

	FRotator Spread = FRotator(FMath::RandRange(-SpreadRadius, SpreadRadius), FMath::RandRange(-SpreadRadius, SpreadRadius), FMath::RandRange(-SpreadRadius, SpreadRadius));

	FVector MuzzleForwardVector = IsValid(Muzzle) ? Muzzle->GetForwardVector() : GetActorForwardVector();

	FVector TargetVector = StartVector + Spread.RotateVector(MuzzleForwardVector) * MaxRange;

	CreateHitReg(StartVector, TargetVector);

	CreateVFX_BulletTracer(StartVector, TargetVector);

	CreateVFX_MuzzleFlash();

	if (IsValid(AnimSeq_Fire))
	{
		Mesh->PlayAnimation(AnimSeq_Fire, false);
	}

	UE_LOG(LogTemp, Log, TEXT("Fire Rifle"));
}

void AWeaponActor_Gun::FireWeapon_Shotgun()
{
	FHitResult Hit;

	FVector StartVector = Muzzle->GetComponentLocation();

	for (int i = 0; i < 8; i++)
	{
		FRotator Spread = FRotator(FMath::RandRange(-SpreadRadius, SpreadRadius), FMath::RandRange(-SpreadRadius, SpreadRadius), FMath::RandRange(-SpreadRadius, SpreadRadius));

		FVector TargetVector = StartVector + Spread.RotateVector(Muzzle->GetForwardVector()) * MaxRange;

		CreateHitReg(StartVector, TargetVector);

		CreateVFX_BulletTracer(StartVector, TargetVector);
	}

	CreateVFX_MuzzleFlash();

	if (IsValid(AnimSeq_Fire))
	{
		Mesh->PlayAnimation(AnimSeq_Fire, false);
	}

	UE_LOG(LogTemp, Log, TEXT("Fire Shotgun"));
}

bool AWeaponActor_Gun::CalcFireDelay_Semi(bool IsHolding)
{
	if (FireDelayCounter <= 0.0f && IsReadyToFire)
	{
		UE_LOG(LogTemp, Log, TEXT("Mode: Semi"));
		
		FireDelayCounter = FireDelay;
		IsReadyToFire = false;
		return true;
	}
	else if (!IsHolding)
	{
		IsReadyToFire = true;
	}
	
	return false;
}

bool AWeaponActor_Gun::CalcFireDelay_Auto(bool IsHolding)
{
	if (FireDelayCounter <= 0.0f)
	{
		FireDelayCounter = FireDelay;
		return true;
	}

	return false;
}

void AWeaponActor_Gun::CreateHitReg(FVector& StartVector, FVector& TargetVector)
{
	FHitResult HitResult;
	FCollisionQueryParams ColQueryParams;
	ColQueryParams.AddIgnoredActor(this);

	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	bool Result = World->LineTraceSingleByChannel(HitResult, StartVector, TargetVector, ECollisionChannel::ECC_Visibility, ColQueryParams, FCollisionResponseParams());

	if (!Result)
	{
		return;
	}

	TargetVector = HitResult.ImpactPoint;

	AActor* TargetActor = HitResult.GetActor();
	if (!IsValid(TargetActor))
	{
		return;
	}

	if (TargetActor->GetClass()->ImplementsInterface(UDamageableInterface::StaticClass()))
	{
		auto DamageableActor = Cast<IDamageableInterface>(HitResult.GetActor());
		DamageableActor->TakeDamage(10.0f);
	}
}

void AWeaponActor_Gun::CreateVFX_BulletTracer(FVector& StartVector, FVector& TargetVector)
{
	UWorld* World = GetWorld();
	if (!IsValid(World) || !IsValid(VFX_BulletTracer))
	{
		return;
	}

	UNiagaraComponent* BulletTracer = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		VFX_BulletTracer,
		StartVector
	);

	BulletTracer->SetVectorParameter(TEXT("StartVector"), StartVector);
	BulletTracer->SetVectorParameter(TEXT("TargetVector"), TargetVector);
}

void AWeaponActor_Gun::CreateVFX_MuzzleFlash()
{
	if (!IsValid(Muzzle) || !IsValid(VFX_MuzzleFlash))
	{
		return;
	}

	UNiagaraComponent* MuzzleFlash = UNiagaraFunctionLibrary::SpawnSystemAttached(
		VFX_MuzzleFlash,
		Muzzle,
		TEXT("MuzzleFlash"),
		FVector(1),
		FRotator(1),
		EAttachLocation::SnapToTarget,
		true,
		true
	);
}

void AWeaponActor_Gun::CalcRPMToDelay()
{
	if (RoundPerMinute != 0)
	{
		FireDelay = 1 / (RoundPerMinute / 60.0f);
	}
	else
	{
		FireDelay = 1.0f;
	}

	FireDelayCounter = -1.0f;
}


