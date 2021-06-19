// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyStates/EnableInputAnimNotifyState.h"

void UEnableInputAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{

}

void UEnableInputAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{

}

void UEnableInputAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		APlayerBase* PlayerBase = Cast<APlayerBase>(MeshComp->GetOwner());
		if (PlayerBase != NULL)
		{
			APlayerController* PlayerController = Cast<APlayerController>(PlayerBase->GetPlayerController());
			if (PlayerController)
			{
				PlayerBase->EnableInput(PlayerController);
			}
		}
	}
}