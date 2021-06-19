// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyStates/CanJumpAnimNotifyState.h"
#include "AnimInstance/AnimInstanceBase.h"

void UCanJumpAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		APlayerBase* PlayerBase = Cast<APlayerBase>(MeshComp->GetOwner());
		if (PlayerBase != NULL)
		{
			//We cannot jump again
			PlayerBase->ActionsManager->SetCanJump(false);
			//We cannot exit ZView
			PlayerBase->GetCustomAnimInstance()->CanExitZView = false;
		}
	}
}

void UCanJumpAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{

}

void UCanJumpAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

}