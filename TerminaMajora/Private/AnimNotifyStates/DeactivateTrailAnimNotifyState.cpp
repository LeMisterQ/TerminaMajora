// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyStates/DeactivateTrailAnimNotifyState.h"

void UDeactivateTrailAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{

}

void UDeactivateTrailAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{

}

void UDeactivateTrailAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		APlayerBase* PlayerBase = Cast<APlayerBase>(MeshComp->GetOwner());
		if (PlayerBase != NULL)
		{
			AWeaponBase* EquippedWeapon = Cast<AWeaponBase>(PlayerBase->GetEquipmentManager()->LH_Equipped_Gear);
			EquippedWeapon->DeactivateTrail();
		}
	}
}