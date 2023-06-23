// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "../../../../../Unreal Engine 5.1/UE_5.1/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"

#include "PlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class AWeaponActor;
class UAnimMontage;
class UInputMappingContext;
class UInputAction;

UCLASS()
class PP2_TPS_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// Basic Components
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* MainCamera;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm_MainCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AWeaponActor* Weapon;

	// Animations
	UPROPERTY(EditAnywhere)
	TMap<FName, UAnimMontage*> Dictionary_AnimMtg;

	// Inputs
	UPROPERTY(EditAnywhere)
	UInputMappingContext* IMC_Player;

	UPROPERTY(EditAnywhere)
	UInputAction* IA_Move_Player;

	UPROPERTY(EditAnywhere)
	UInputAction* IA_Look_Player;

	UPROPERTY(EditAnywhere)
	UInputAction* IA_Aim_Player;

	UPROPERTY(EditAnywhere)
	UInputAction* IA_Fire_Player;

	// Vectors
	UPROPERTY(VisibleAnywhere)
	FVector OriginCameraOffset;

	UPROPERTY(VisibleAnywhere)
	FVector AimCameraOffset;

	// Values
	UPROPERTY(VisibleAnywhere)
	float OriginCameraArmLength;

	UPROPERTY(VisibleAnywhere)
	float AimCameraArmLength;

	UPROPERTY(EditAnywhere)
	float MoveSpeed_Default;

	UPROPERTY(EditAnywhere)
	float MoveSpeed_Aim;

	float MoveSpeed;

	float AimLerpValue;

	UPROPERTY(BlueprintReadOnly)
	bool IsHoldingWeapon;

	UPROPERTY(BlueprintReadOnly)
	bool IsAiming;

	bool IsFiring;

public:

	UFUNCTION(BlueprintCallable)
	FTransform GetSocketLocation(FName SocketName);

	// Methods
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void AimOn(const FInputActionValue& Value);
	void AimOff(const FInputActionValue& Value);
	void Fire(const FInputActionValue& Value);
	void CeaseFire(const FInputActionValue& Value);
};
