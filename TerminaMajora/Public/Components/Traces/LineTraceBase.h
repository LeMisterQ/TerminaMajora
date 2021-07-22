// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "LineTraceBase.generated.h"

//Decoration, there is going to be a pointer on this
class APlayerBase;
class UAnimInstanceBase;
class ALandscape;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TERMINAMAJORA_API ULineTraceBase : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULineTraceBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void FootStepsTrace(APlayerBase* OwnerPlayer, FName RightFootSKT, FName LeftFootSKT, bool IsRightStep);

	UFUNCTION(BlueprintCallable)
		FString FloorTypeCheck(APlayerBase* OwnerPlayer, FName RightFootSKT);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
		float TraceDuration = 2.0f;

private:

	//Declaring Start and End trace points
	FHitResult Hit;
	FVector TraceStart;
	FVector TraceEnd;
	FCollisionQueryParams TraceParams;

		
};
