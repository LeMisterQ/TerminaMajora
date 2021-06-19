// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyStates/DisableInputAnimNotifyState.h"

void UDisableInputAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{

	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		APlayerBase* PlayerBase = Cast<APlayerBase>(MeshComp->GetOwner());
		if (PlayerBase != NULL)
		{
			APlayerController* PlayerController = Cast<APlayerController>(PlayerBase->GetPlayerController());
			if (PlayerController)
			{
				PlayerBase->DisableInput(PlayerController);
			}
			
		}
	}
}

void UDisableInputAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{

}

void UDisableInputAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

}