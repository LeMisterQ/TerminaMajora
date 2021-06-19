// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyStates/EquipAnimNotifyState.h"

void UEquipAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{

	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		APlayerBase* PlayerBase = Cast<APlayerBase>(MeshComp->GetOwner());
		if (PlayerBase != NULL)
		{
			PlayerBase->EquipmentManager->Equip_SwordShield1H();
		}
	}
}

void UEquipAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	/*GEngine->AddOnScreenDebugMessage(-1, 4.5f, FColor::Blue, __FUNCTION__);*/
}

void UEquipAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	/*GEngine->AddOnScreenDebugMessage(-1, 4.5f, FColor::Yellow, __FUNCTION__);*/
}
