// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyStates/HitAnimNotifyState.h"

void UHitAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{

}

void UHitAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		APlayerBase* PlayerBase = Cast<APlayerBase>(MeshComp->GetOwner());
		if (PlayerBase != NULL)
		{
			//Calling the Trace for damage registration
			PlayerBase->EquipmentManager->LH_Equipped_Gear->TriggerAdvancedHit();
		}
	}
}

void UHitAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

}
