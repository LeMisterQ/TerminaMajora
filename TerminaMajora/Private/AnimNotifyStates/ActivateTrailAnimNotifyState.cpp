// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyStates/ActivateTrailAnimNotifyState.h"

void UActivateTrailAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{

	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		APlayerBase* PlayerBase = Cast<APlayerBase>(MeshComp->GetOwner());
		if (PlayerBase != NULL)
		{
			AWeaponBase* EquippedWeapon = Cast<AWeaponBase>(PlayerBase->GetEquipmentManager()->LH_Equipped_Gear);
			EquippedWeapon->ActivateTrail();
		}
	}
}

void UActivateTrailAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{

}

void UActivateTrailAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

}