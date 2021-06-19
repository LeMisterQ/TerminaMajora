// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gear/GearBase.h"
#include "ShieldBase.generated.h"

/**
 *
 */
UCLASS()
class TERMINAMAJORA_API AShieldBase : public AGearBase
{
	GENERATED_BODY()

		//Expose a component to Blueprint (always this setup for that purpose)
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gear|Shield", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* Shield_1H;

public:
	//Constructor
	AShieldBase();

};
