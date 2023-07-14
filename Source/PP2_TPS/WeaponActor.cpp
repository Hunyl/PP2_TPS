// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActor.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"

#include "PlayerCharacter.h"

// Sets default values
AWeaponActor::AWeaponActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));

	Mesh->SetupAttachment(RootComponent);
	Collision->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AWeaponActor::OnComponentBeginOverlap);
}

// Called every frame
void AWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponActor::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Log, TEXT("Begin Overlap..."));

	TWeakObjectPtr<APlayerCharacter> Player = CastChecked<APlayerCharacter>(OtherActor);
	if (!Player.IsValid())
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Acquired a Weapon"));

	USkeletalMeshComponent* PlayerMesh = Player->GetMesh();
	if (!IsValid(PlayerMesh))
	{
		return;
	}

	EAttachmentRule AttachRule = EAttachmentRule::SnapToTarget;
	FAttachmentTransformRules AttachTransFormRules = FAttachmentTransformRules(AttachRule, false);
	this->AttachToComponent(PlayerMesh, AttachTransFormRules, TEXT("socket_weapon"));

	Collision->SetCollisionProfileName(TEXT("NoCollision"));

	TWeakObjectPtr<AWeaponActor> PlayerWeapon_Current = Player->Weapon;
	if (PlayerWeapon_Current.IsValid())
	{
		PlayerWeapon_Current->Destroy();
	}

	Player->Weapon = this;
}

void AWeaponActor::Attack(bool IsHolding)
{
	// UE_LOG(LogTemp, Log, TEXT("Begin Attack Sequence"));
}


