// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SphereTraceBase.generated.h"

/// ENUMS ///
UENUM(BlueprintType)
enum class ESKT_Trace : uint8 {
	Default			UMETA(DisplayName = "Default"),
	TraceStart		UMETA(DisplayName = "SKT_trace_start"),
	TraceEnd		UMETA(DisplayName = "SKT_trace_end"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TERMINAMAJORA_API USphereTraceBase : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USphereTraceBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		AGearBase* GetOwnerGearBase();

	UFUNCTION(BlueprintCallable)
		void GenerateAdvancedHit(USkeletalMeshComponent* OwnerSKMesh);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
		float TraceRadius = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
		float TraceDuration = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Traces")
		TArray<FName> SKT_Trace_Array;


		
};
