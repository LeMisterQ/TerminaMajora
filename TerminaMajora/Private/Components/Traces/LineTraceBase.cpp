// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Traces/LineTraceBase.h"
#include "Engine.h"
#include "Landscape.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Characters/Player/PlayerBase.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
ULineTraceBase::ULineTraceBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULineTraceBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void ULineTraceBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULineTraceBase::FootStepsTrace(APlayerBase* OwnerPlayer, FName RightFootSKT, FName LeftFootSKT, bool IsRightStep)
{
	
	if (IsRightStep)
	{

		TraceStart = OwnerPlayer->GetMesh()->GetSocketLocation(RightFootSKT) + FVector(0, 0, 50);
		TraceEnd = OwnerPlayer->GetMesh()->GetSocketLocation(RightFootSKT) + FVector(0, 0, -50);
	}
	else
	{

		TraceStart = OwnerPlayer->GetMesh()->GetSocketLocation(LeftFootSKT) + FVector(0, 0, 50);
		TraceEnd = OwnerPlayer->GetMesh()->GetSocketLocation(LeftFootSKT) + FVector(0, 0, -50);
	}

	//Enable Physical Material detection ability
	TraceParams.bTraceComplex = false;
	TraceParams.bReturnPhysicalMaterial = true;

	//Change this to enable visual debug
	TraceDuration = 0.0f;

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
			//Grass
			case SurfaceType1:
			{/*GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Hit: Grass")));*/
				OwnerPlayer->SoundManager->PlaySoundAtLoc(OwnerPlayer->SoundManager->Footstep_Grass, OwnerPlayer->GetActorLocation());
				break;
			}

			//Stone
			case SurfaceType2:
			{	/*GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Hit: Rock")));*/
				OwnerPlayer->SoundManager->PlaySoundAtLoc(OwnerPlayer->SoundManager->Footstep_Stone, OwnerPlayer->GetActorLocation());
				break;
			}

			//Wood
			case SurfaceType3:
			{	/*GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Hit: Wood")));*/
				OwnerPlayer->SoundManager->PlaySoundAtLoc(OwnerPlayer->SoundManager->Footstep_Wood, OwnerPlayer->GetActorLocation());
				break;
			}

			//Dirt
			case SurfaceType4:
			{	/*GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Hit: Dirt")));*/
				OwnerPlayer->SoundManager->PlaySoundAtLoc(OwnerPlayer->SoundManager->Footstep_Dirt, OwnerPlayer->GetActorLocation());
				break;
			}

			//TallGrass
			case SurfaceType5:
			{	/*GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Hit: TallGrass")));*/
				OwnerPlayer->SoundManager->PlaySoundAtLoc(OwnerPlayer->SoundManager->Footstep_TallGrass, OwnerPlayer->GetActorLocation());
				break;
			}

			//Mud
			case SurfaceType6:
			{	/*GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Hit: TallGrass")));*/
				OwnerPlayer->SoundManager->PlaySoundAtLoc(OwnerPlayer->SoundManager->Footstep_Mud, OwnerPlayer->GetActorLocation());
				break;
			}
		}

	}
}

FString ULineTraceBase::FloorTypeCheck(APlayerBase* OwnerPlayer, FName RightFootSKT)
{
	TraceStart = OwnerPlayer->GetMesh()->GetSocketLocation(RightFootSKT) + FVector(0, 0, 50);
	TraceEnd = OwnerPlayer->GetMesh()->GetSocketLocation(RightFootSKT) + FVector(0, 0, -50);

	TraceParams.bTraceComplex = false;

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams);
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Orange, false, TraceDuration);

	if (bHit)
	{
		//Checking if Hit Actor is of type Landscape
		FString LandscapeType(TEXT("landscape"));
		if (Hit.GetActor()->GetClass()->GetName().Contains(LandscapeType, ESearchCase::IgnoreCase))
		{
			/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("LandScape detected"));*/
		}

		//Return the Hit Actor Name (FString)
		return Hit.GetActor()->GetClass()->GetName();
	}
	else
	{
		return FString("no_hit");
	}
}

