// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Player/PlayerBase.h"
#include "Gear/WeaponBase.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "DeactivateTrailAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class TERMINAMAJORA_API UDeactivateTrailAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
	
};
