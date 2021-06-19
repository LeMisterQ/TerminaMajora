// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstanceBase.generated.h"

//Forward Declaration
class APawn;
class APlayerBase;
class UPawnMovementComponent;
class APlayerControllerBase;

UCLASS()
class TERMINAMAJORA_API UAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

public:

	UAnimInstanceBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
		float Speed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
		float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
		float SpeedForward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
		float SpeedRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
		float FwdAxisValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
		float RghtAxisValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
		bool IsInAir = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
		bool CanExitZView = true;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Generic")
		class APawn* PawnOwner;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Generic")
		class APlayerBase* OwnerPlayer;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Generic")
		class APlayerControllerBase* OwnerPlayerController;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


};
