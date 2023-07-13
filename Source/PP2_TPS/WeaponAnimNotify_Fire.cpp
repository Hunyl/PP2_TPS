// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponAnimNotify_Fire.h"

#include "WeaponActor_Gun.h"

void UWeaponAnimNotify_Fire::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!IsValid(MeshComp))
	{
		return;
	}

	if (AWeaponActor_Gun* Weapon = Cast<AWeaponActor_Gun>(MeshComp->GetOwner()))
	{
		if (IsValid(Weapon->Actor_Case))
		{
			FActorSpawnParameters SpawnParams;

			GetWorld()->SpawnActor<AActor>(Weapon->Actor_Case, MeshComp->GetSocketLocation(TEXT("AmmoEject")), MeshComp->GetComponentRotation(), SpawnParams);
		}
	}

}