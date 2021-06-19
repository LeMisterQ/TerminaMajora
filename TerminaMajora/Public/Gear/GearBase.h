// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "Components/Traces/SphereTraceBase.h"
#include "GearBase.generated.h"

UCLASS()
class TERMINAMAJORA_API AGearBase : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gear", meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* MuzzlePoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gear", meta = (AllowPrivateAccess = "true"))
		class USceneComponent* RootComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gear", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BaseSM;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gear", meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* BaseSK;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trace", meta = (AllowPrivateAccess = "true"))
		class USphereTraceBase* SphereTrace;

public:
	// Sets default values for this actor's properties
	AGearBase();

	UFUNCTION(BlueprintCallable)
	APlayerBase* GetPlayerOwner();

	//Set corresponding SM or SK Mesh
	void SetSMMesh();
	virtual void SetSMMesh_Implementation(UStaticMesh* FetchedSM);

	void SetSKMesh();
	virtual void SetSKMesh_Implementation(USkeletalMesh* FetchedSK);

	UFUNCTION(BlueprintCallable)
	UStaticMeshComponent* GetSMMesh();

	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetSKMesh();

	UFUNCTION(BlueprintCallable)
	USphereTraceBase* GetSphereTrace_SCN();

	UFUNCTION()
		void TriggerAdvancedHit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:


};