// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gear/GearBase.h"
#include "NiagaraComponent.h"
#include "WeaponBase.generated.h"

/**
 * 
 */
UCLASS()
class TERMINAMAJORA_API AWeaponBase : public AGearBase
{
	GENERATED_BODY()

public:
	//Constructor
	AWeaponBase();

	//Niagara Components
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trail")
	UNiagaraComponent* SMNiagaraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trail")
		UNiagaraComponent* SKNiagaraComponent;

	//Trail Activation
	UFUNCTION(BlueprintCallable, Category = "Trail")
	void ActivateTrail();

	UFUNCTION(BlueprintCallable, Category = "Trail")
	void DeactivateTrail();
};
