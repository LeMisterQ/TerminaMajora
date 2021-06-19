// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Player/PlayerBase.h"
#include "Characters/Player/PlayerControllerBase.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "DisableInputAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class TERMINAMAJORA_API UDisableInputAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
	
};
