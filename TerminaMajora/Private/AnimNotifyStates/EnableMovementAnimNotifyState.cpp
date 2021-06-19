// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyStates/EnableMovementAnimNotifyState.h"
#include "GameFramework/CharacterMovementComponent.h"

void UEnableMovementAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	
}

void UEnableMovementAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{

}

void UEnableMovementAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		APlayerBase* PlayerBase = Cast<APlayerBase>(MeshComp->GetOwner());
		if (PlayerBase != NULL)
		{
			//We UNconstrain the movement on ALL AXIS when this ANIMNOTSTATE is triggered At Begin
			//UPawnMovementComponent* const MovementComponent = PlayerBase->GetMovementComponent();
			//MovementComponent->Activate();
			PlayerBase->GetMovementComp()->MaxWalkSpeed = PlayerBase->DefaultWalkSpeed;
		}
	}
}