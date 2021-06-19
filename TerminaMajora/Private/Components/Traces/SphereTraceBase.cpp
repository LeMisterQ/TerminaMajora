// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Traces/SphereTraceBase.h"
#include "Engine.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Characters/Player/PlayerBase.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
USphereTraceBase::USphereTraceBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//Populating the SKT Names Array we will use for the traces
	FName Arr[] = { TEXT("SKT_trace_start"), TEXT("SKT_trace_end") };
	SKT_Trace_Array.Append(Arr, UE_ARRAY_COUNT(Arr));
}


// Called when the game starts
void USphereTraceBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USphereTraceBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

AGearBase* USphereTraceBase::GetOwnerGearBase()
{
	AGearBase* OwnerGearBase = Cast<AGearBase>(this->GetOwner());
	return OwnerGearBase;
}

//Called on Request
void USphereTraceBase::GenerateAdvancedHit(USkeletalMeshComponent* OwnerSKMesh)
{
	//Get Reference to our Owner SK Mesh

	/*
	const FVector Start = OwnerSKMesh->GetSocketLocation(SKT_Trace_Array[0]);
	const FVector End = OwnerSKMesh->GetSocketLocation(SKT_Trace_Array[1]);

	//Actors Generating a Hit reaction
	TArray<TEnumAsByte<EObjectTypeQuery>> ActorsToSweep;
	//ObjectTypeQuery3 == PAWN OBJECT, so we want only Pawn OBJECT to be able to generate a Hit reaction
	ActorsToSweep.Add(EObjectTypeQuery::ObjectTypeQuery3);

	//Getting all the actors corresponding to PlayerBase and set them as Actors to Ignore;
	TArray<AActor*> ActorsToIgnore;
	TSubclassOf<APlayerBase> ClassToIgnore;
	ClassToIgnore = APlayerBase::StaticClass();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToIgnore, ActorsToIgnore);
	
	//Storing Hit Result so we can use them to apply damage or spawn VFX for example
	TArray<FHitResult> HitArray;

	const bool Hit = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Start, End, TraceRadius, ActorsToSweep,
		false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitArray, true, FLinearColor::Red, FLinearColor::Green, TraceDuration);

	if (Hit)
	{
		for (const FHitResult HitResult : HitArray)
		{
			
			//Apply Damage on HitActor Class here
			//



		}
	}
	*/
	
}

