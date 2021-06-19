// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyStates/UnequipAnimNotifyState.h"

void UUnequipAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{

	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		APlayerBase* PlayerBase = Cast<APlayerBase>(MeshComp->GetOwner());
		if (PlayerBase != NULL)
		{
			PlayerBase->EquipmentManager->UnEquip_SwordShield1H();
		}
	}
}

void UUnequipAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{

}

void UUnequipAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

}
