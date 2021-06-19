// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/AnimInstanceBase.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/Player/PlayerBase.h"
#include "Characters/Player/PlayerControllerBase.h"

UAnimInstanceBase::UAnimInstanceBase()
{
	this->RootMotionMode = ERootMotionMode::RootMotionFromEverything;
}

void UAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	PawnOwner = TryGetPawnOwner();
	if (PawnOwner)
	{
		Speed = PawnOwner->GetVelocity().Size();
		IsInAir = PawnOwner->GetMovementComponent()->IsFalling();

		OwnerPlayer = Cast<APlayerBase>(PawnOwner);

		FVector PlayerVelocity = OwnerPlayer->GetVelocity();
		FRotator PlayerRotation = OwnerPlayer->GetActorRotation();
		Direction = this->CalculateDirection(PlayerVelocity, PlayerRotation);

		OwnerPlayerController = OwnerPlayer->GetPlayerController();
		if(OwnerPlayerController)
		{
			FwdAxisValue = OwnerPlayerController->GetAxisValues().X;
			RghtAxisValue = OwnerPlayerController->GetAxisValues().Y;
		}

	}
}
