// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Gear/GearBase.h"
#include "Gear/WeaponBase.h"
#include "InventoryPlayer.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventoryPlayer : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TERMINAMAJORA_API IInventoryPlayer
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface")
	void TransmitMessage();


};
