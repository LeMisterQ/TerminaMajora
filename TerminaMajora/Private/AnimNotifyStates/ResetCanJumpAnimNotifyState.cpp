// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyStates/ResetCanJumpAnimNotifyState.h"
#include "AnimInstance/AnimInstanceBase.h"

void UResetJumpTypeAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{

}

void UResetJumpTypeAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{

}

void UResetJumpTypeAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		APlayerBase* PlayerBase = Cast<APlayerBase>(MeshComp->GetOwner());
		if (PlayerBase != NULL)
		{			
			PlayerBase->ActionsManager->SetJumpType(EJumpTypes::Default);			
			//We can jump again
			PlayerBase->ActionsManager->SetCanJump(true);
			//We can exit ZView
			PlayerBase->GetCustomAnimInstance()->CanExitZView = true;
			
		}
	}
}
