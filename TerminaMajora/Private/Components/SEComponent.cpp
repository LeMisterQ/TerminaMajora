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

	//...
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
	//Declaring Start and End trace points
	FHitResult Hit;
	FVector TraceStart;
	FVector TraceEnd;

	if (IsRightStep)
	{

		TraceStart = OwnerPlayer->GetMesh()->GetSocketLocation(TEXT("foot_r")) + FVector(0, 0, 50);
		TraceEnd = OwnerPlayer->GetMesh()->GetSocketLocation(TEXT("foot_r")) + FVector(0, 0, -50);
	}
	else
	{

		TraceStart = OwnerPlayer->GetMesh()->GetSocketLocation(TEXT("foot_l")) + FVector(0, 0, 50);
		TraceEnd = OwnerPlayer->GetMesh()->GetSocketLocation(TEXT("foot_l")) + FVector(0, 0, -50);
	}

	//LineTrace
	FCollisionQueryParams TraceParams;
	TraceParams.bTraceComplex = false;
	TraceParams.bReturnPhysicalMaterial = true;

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams);
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Orange, false, TraceDuration);
	
	if (bHit)
	{
		DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5), FColor::Emerald, false, TraceDuration);
		//Getting the Phys Mat from Hit result
		UPhysicalMaterial* PhysMat = Hit.PhysMaterial.Get();
		EPhysicalSurface SurfaceT = PhysMat->SurfaceType;

		switch (SurfaceT)
		{
			case SurfaceType1:
			{/*GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Hit: Grass")));*/
				PlaySoundAtLoc(Footstep_Grass, OwnerPlayer->GetActorLocation());
				break;
			}

			case SurfaceType2:
			{	/*GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Hit: Rock")));*/
				PlaySoundAtLoc(Footstep_Stone, OwnerPlayer->GetActorLocation());
				break;
			}

			case SurfaceType3:
			{	/*GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Hit: Wood")));*/
				PlaySoundAtLoc(Footstep_Wood, OwnerPlayer->GetActorLocation());
				break;
			}

			case SurfaceType4:
			{	/*GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Hit: Dirt")));*/
				PlaySoundAtLoc(Footstep_Dirt, OwnerPlayer->GetActorLocation());
				break;
			}

			case SurfaceType5:
			{	/*GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Hit: TallGrass")));*/
				PlaySoundAtLoc(Footstep_TallGrass, OwnerPlayer->GetActorLocation());
				break;
			}
		}

	}

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

