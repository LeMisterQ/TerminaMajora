// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyStates/DisableMovementAnimNotifyState.h"
#include "GameFramework/CharacterMovementComponent.h"

void UDisableMovementAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{

	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		APlayerBase* PlayerBase = Cast<APlayerBase>(MeshComp->GetOwner());
		if (PlayerBase != NULL)
		{
			//We constrain the movement on ALL AXIS when this ANIMNOTSTATE is triggered At Begin
			//UPawnMovementComponent* const MovementComponent = PlayerBase->GetMovementComponent();
			//MovementComponent->Deactivate();
			PlayerBase->GetMovementComp()->MaxWalkSpeed = 10.0f;
		}
	}
}

void UDisableMovementAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{

}

void UDisableMovementAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

}
