// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Animation/AnimMontage.h"

#include "WeaponActor.h"
#include "WeaponActor_Gun.h"

#include "../../../../../Unreal Engine 5.1/UE_5.1/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "../../../../../Unreal Engine 5.1/UE_5.1/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../../Unreal Engine 5.1/UE_5.1/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/InputMappingContext.h"

static const FName AnimMtg_Fire_Rifle(TEXT("AnimMtg_Fire_Rifle"));
static const FName AnimMtg_Fire_Shotgun(TEXT("AnimMtg_Fire_Shotgun"));

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	SpringArm_MainCamera = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm_MainCamera"));

	OriginCameraOffset = FVector(0.0f, 30.0f, 60.0f);
	AimCameraOffset = FVector(0.0f, 60.0f, 55.0f);

	OriginCameraArmLength = 125.0f;
	AimCameraArmLength = 100.0f;

	AimLerpValue = 0.0f;

	IsHoldingWeapon = false;

	SpringArm_MainCamera->SetupAttachment(RootComponent);
	MainCamera->SetupAttachment(SpringArm_MainCamera);

	MainCamera->bUsePawnControlRotation = false;

	SpringArm_MainCamera->TargetArmLength = OriginCameraArmLength;
	SpringArm_MainCamera->SocketOffset = OriginCameraOffset;
	SpringArm_MainCamera->bUsePawnControlRotation = true;
	
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC_Player, 0);
		}
	}

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	MoveSpeed = MoveSpeed_Default;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(Weapon))
	{
		IsHoldingWeapon = true;
	}
	else
	{
		IsHoldingWeapon = true;
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Move_Player, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(IA_Look_Player, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(IA_Aim_Player, ETriggerEvent::Triggered, this, &APlayerCharacter::AimOn);
		EnhancedInputComponent->BindAction(IA_Aim_Player, ETriggerEvent::Completed, this, &APlayerCharacter::AimOff);
		EnhancedInputComponent->BindAction(IA_Fire_Player, ETriggerEvent::Triggered, this, &APlayerCharacter::Fire);
		EnhancedInputComponent->BindAction(IA_Fire_Player, ETriggerEvent::Completed, this, &APlayerCharacter::CeaseFire);
	}

}

FTransform APlayerCharacter::GetSocketLocation(FName SocketName)
{
	if (!IsValid(Weapon))
	{
		return FTransform();
	}

	if (!IsValid(Weapon->Mesh))
	{
		return FTransform();
	}

	return Weapon->Mesh->GetSocketTransform(SocketName, ERelativeTransformSpace::RTS_World);
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D Movement = Value.Get<FVector2D>();

	FRotator Rotation = Controller->GetControlRotation();

	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, Movement.Y * MoveSpeed);

	FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, Movement.X * MoveSpeed);
	
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D Look = Value.Get<FVector2D>();

	AddControllerYawInput(Look.X);
	AddControllerPitchInput(Look.Y);
}

void APlayerCharacter::AimOn(const FInputActionValue& Value)
{
	AimLerpValue += Value.Get<float>() * 7.5f;

	if (AimLerpValue >= 1.0f)
	{
		AimLerpValue = 1.0f;
	}

	MoveSpeed = MoveSpeed_Aim;

	IsAiming = true;

	FVector MedianOffset = FMath::Lerp(OriginCameraOffset, AimCameraOffset, AimLerpValue);
	float MedianLength = FMath::Lerp(OriginCameraArmLength, AimCameraArmLength, AimLerpValue);
	float MedianFOV = FMath::Lerp(90.0f, 75.0f, AimLerpValue);

	if (IsValid(SpringArm_MainCamera))
	{
		SpringArm_MainCamera->SocketOffset = MedianOffset;
		SpringArm_MainCamera->TargetArmLength = MedianLength;
	}
	
	if (IsValid(MainCamera))
	{
		MainCamera->FieldOfView = MedianFOV;
	}
}

void APlayerCharacter::AimOff(const FInputActionValue& Value)
{
	AimLerpValue = 0.0f;

	MoveSpeed = MoveSpeed_Default;

	IsAiming = false;

	if (IsValid(SpringArm_MainCamera))
	{
		SpringArm_MainCamera->SocketOffset = OriginCameraOffset;
		SpringArm_MainCamera->TargetArmLength = OriginCameraArmLength;
	}

	if (IsValid(MainCamera))
	{
		MainCamera->FieldOfView = 90.0f;
	}
}

void APlayerCharacter::Fire(const FInputActionValue& Value)
{
	IsFiring = true;

	if (!IsAiming)
	{
		UE_LOG(LogTemp, Log, TEXT("Not Aiming!"));
		return;
	}

	if (!IsValid(Weapon))
	{
		UE_LOG(LogTemp, Log, TEXT("No Weapon!"));
		return;
	}

	if (IsValid(Weapon))
	{
		Weapon->Attack(IsFiring);
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!IsValid(AnimInstance))
	{
		return;
	}

	AWeaponActor_Gun* Weapon_Gun = CastChecked<AWeaponActor_Gun>(Weapon);
	if (!IsValid(Weapon_Gun))
	{
		return;
	}

	bool Checked_Rifle = Weapon_Gun->WeaponType == EWeaponType::Rifle && Dictionary_AnimMtg.Contains(AnimMtg_Fire_Rifle);
	bool Checked_Shotgun = Weapon_Gun->WeaponType == EWeaponType::Shotgun && Dictionary_AnimMtg.Contains(AnimMtg_Fire_Shotgun);

	if (Checked_Rifle)
	{
		AnimInstance->Montage_Play(Dictionary_AnimMtg.FindChecked(AnimMtg_Fire_Rifle), 1.0f);
	}
	else if (Checked_Shotgun)
	{
		AnimInstance->Montage_Play(Dictionary_AnimMtg.FindChecked(AnimMtg_Fire_Shotgun), 1.0f);
	}
}

void APlayerCharacter::CeaseFire(const FInputActionValue& Value)
{
	IsFiring = false;

	if (!IsAiming)
	{
		UE_LOG(LogTemp, Log, TEXT("Not Aiming!"));
		return;
	}

	if (!IsValid(Weapon))
	{
		UE_LOG(LogTemp, Log, TEXT("No Weapon!"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Completed"));

	if (IsValid(Weapon))
	{
		Weapon->Attack(IsFiring);
	}
}

