// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "../../../../../Unreal Engine 5.1/UE_5.1/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"

#include "PlayerCharacter.generated.h"

// Forward Declarations
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
	
	APlayerCharacter();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// Basic Components
	UPROPERTY(VisibleAnywhere, Category = "Basic Components")
	UCameraComponent* MainCamera;

	UPROPERTY(VisibleAnywhere, Category = "Basic Components")
	USpringArmComponent* SpringArm_MainCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Basic Components")
	AWeaponActor* Weapon;

	// Animations
	UPROPERTY(EditAnywhere, Category = "Animation - Montages")
	TMap<FName, UAnimMontage*> Dictionary_AnimMtg;

	// Inputs
	UPROPERTY(EditAnywhere, Category = "Inputs")
	UInputMappingContext* IMC_Player;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	UInputAction* IA_Move_Player;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	UInputAction* IA_Look_Player;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	UInputAction* IA_Aim_Player;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	UInputAction* IA_Fire_Player;

	// Vectors
	UPROPERTY(VisibleAnywhere, Category = "Property - Vectors")
	FVector OriginCameraOffset;

	UPROPERTY(VisibleAnywhere, Category = "Property - Vectors")
	FVector AimCameraOffset;

	// Values
	UPROPERTY(VisibleAnywhere, Category = "Property - Values")
	float OriginCameraArmLength;

	UPROPERTY(VisibleAnywhere, Category = "Property - Values")
	float AimCameraArmLength;

	UPROPERTY(EditAnywhere, Category = "Basic Status")
	float MoveSpeed_Default;

	UPROPERTY(EditAnywhere, Category = "Basic Status")
	float MoveSpeed_Aim;

	float MoveSpeed;

	float AimLerpValue;

	UPROPERTY(BlueprintReadOnly)
	bool IsHoldingWeapon;

	UPROPERTY(BlueprintReadOnly)
	bool IsAiming;

	bool IsFiring;

public:
	// Unreal Functions
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
