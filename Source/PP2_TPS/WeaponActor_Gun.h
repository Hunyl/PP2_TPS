// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponActor.h"
#include "WeaponActor_Gun.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

DECLARE_DELEGATE(FFireSequence);
DECLARE_DELEGATE_RetVal_OneParam(bool, FFireDelaySequence, bool);

UENUM()
enum class EWeaponType : uint8
{
	Pistol,
	Rifle,
	Shotgun,
};

UENUM()
enum class EFireModeType : uint8
{
	Semi,
	Auto,
};

UCLASS()
class PP2_TPS_API AWeaponActor_Gun : public AWeaponActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AWeaponActor_Gun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	// Custom Delegates, Events and Functions
	FFireSequence FireSequence;

	FFireDelaySequence FireDelaySequence;

	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere)
	EFireModeType FireModeType;

	// Components
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* Niagara;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* VFX_BulletTracer;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* VFX_MuzzleFlash;

	UPROPERTY(EditAnywhere)
	UAnimSequence* AnimSeq_Fire;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> Actor_Case;

	// Values: Weapon Stats
	UPROPERTY(EditAnywhere)
	float MaxRange;

	UPROPERTY(EditAnywhere)
	float SpreadRadius;

	UPROPERTY(EditAnywhere)
	int RoundPerMinute;

public:

	// Methods
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit) override;

	virtual void Attack(bool IsHolding) override;

	bool IsOnFireDelay();

private:

	// Components
	USceneComponent* Muzzle;
	
	// Values
	float FireDelay;

	float FireDelayCounter;

	bool IsReadyToFire;

private:

	// Unreal Functions
	UFUNCTION()
	void Fire();

	UFUNCTION()
	void FireWeapon_Pistol();

	UFUNCTION()
	void FireWeapon_Rifle();

	UFUNCTION()
	void FireWeapon_Shotgun();

	UFUNCTION()
	bool CalcFireDelay_Semi(bool IsHolding);

	UFUNCTION()
	bool CalcFireDelay_Auto(bool IsHolding);

	// Methods
	void CreateHitTrace(FVector& StartVector, FVector& TargetVector);

	void CalcRPMToDelay();
};
