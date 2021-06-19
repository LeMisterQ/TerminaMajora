// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gear/GearBase.h"
#include "SheathBase.generated.h"

/**
 *
 */
UCLASS()
class TERMINAMAJORA_API ASheathBase : public AGearBase
{
	GENERATED_BODY()

		//Expose a component to Blueprint (always this setup for that purpose)
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gear|Sheath", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* Sheath_Back;

public:
	//Constructor
	ASheathBase();

};
