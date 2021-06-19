// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Player/PlayerBase.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "FootstepAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class TERMINAMAJORA_API UFootstepAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep")
	bool IsRightStep = false;
};
