// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "Sound/SoundCue.h"
#include "Components/ActorComponent.h"
#include "Components/Traces/LineTraceBase.h"
#include "SEComponent.generated.h"

//Decoration, there is going to be a pointer on this
class APlayerBase;

USTRUCT(BlueprintType)
struct FLinkSoundEffects : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FName, USoundCue*> LinkSounds;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TERMINAMAJORA_API USEComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USEComponent();

	UFUNCTION()
		void PlaySoundBase(USoundBase* SoundBaseToPlay);

	UFUNCTION()
		void PlaySoundAtLoc(USoundCue* SoundCueToPlay, FVector WhereToPlay);

	UFUNCTION()
		void ExecFootStep(bool IsRightStep = false);

	//Function Getting the SoundsCues DataTable so we can use it
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sounds")
		class UDataTable* FLinkSoundEffects;

	//ShieldIN and Shield OUT are out of any montages, since they correspond to a stance (ECombatType)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sounds|Shield")
		class USoundCue* ShieldOutSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sounds|Shield")
		class USoundCue* ShieldInSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds|View")
		class USoundBase* ZViewSoundToPlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds|View")
		class USoundBase* ZViewStandardSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds|View")
		class USoundBase* ZViewDungeonSound;

	UFUNCTION()
		void SetZViewSound();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds|Trace")
		float TraceDuration = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sounds|Footsteps")
		class USoundCue* Footstep_Grass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sounds|Footsteps")
		class USoundCue* Footstep_Stone;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sounds|Footsteps")
		class USoundCue* Footstep_Wood;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sounds|Footsteps")
		class USoundCue* Footstep_Dirt;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sounds|Footsteps")
		class USoundCue* Footstep_TallGrass;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Reference")
		class ULineTraceBase* LineTrace;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	//Sets the Owner to the Player 
	APlayerBase* OwnerPlayer;
};
