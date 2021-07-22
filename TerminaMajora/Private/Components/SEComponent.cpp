// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SEComponent.h"
#include "Characters/Player/PlayerBase.h"
#include "DrawDebugHelpers.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Chaos/ChaosEngineInterface.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
USEComponent::USEComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//Attach LineTrace for footstep and ledge detection
	LineTrace = CreateDefaultSubobject<ULineTraceBase>(TEXT("LineTrace"));
}


void USEComponent::PlaySoundBase(USoundBase* SoundBaseToPlay)
{
	if (SoundBaseToPlay)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundBaseToPlay);
	}
	else
	{
		/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Sound is nullptr !"));*/
	}
	
}

void USEComponent::PlaySoundAtLoc(USoundCue* SoundCueToPlay, FVector WhereToPlay)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundCueToPlay, WhereToPlay);
}

void USEComponent::ExecFootStep(bool IsRightStep)
{
	LineTrace->FootStepsTrace(OwnerPlayer, FName("foot_r"), FName("foot_l"), IsRightStep);

}

void USEComponent::SetZViewSound()
{
	if (OwnerPlayer->IsInDungeon == false)
	{
		ZViewSoundToPlay = ZViewStandardSound;
	}
	else
	{
		ZViewSoundToPlay = ZViewDungeonSound;
	}
}

// Called when the game starts
void USEComponent::BeginPlay()
{
	Super::BeginPlay();

	//Declaring OwnerPlayer firstly
	OwnerPlayer = Cast<APlayerBase>(this->GetOwner());

	//Sets Various sounds at initialization
	SetZViewSound();
	
}


// Called every frame
void USEComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

