// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gear/GearBase.h"
#include "Components/ActorComponent.h"
#include "EquipmentComponent.generated.h"

/// ENUMS ///
UENUM(BlueprintType)
enum class ECombatType : uint8 {
	Unarmed			UMETA(DisplayName = "Unarmed"),
	Sword1H			UMETA(DisplayName = "Sword 1H"),
	Sword2H			UMETA(DisplayName = "Sword 2H"),
	Shield			UMETA(DisplayName = "Shield"),
	Charge			UMETA(DisplayName = "Charge"),
};

/// EVENTS DISPATCHERS ///
//OnCombatTypeChanged Event Dispatcher
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombatTypeChanged, ECombatType, CombatType);

//Decoration, there is going to be a pointer on this
class APlayerBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TERMINAMAJORA_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipmentComponent();

	//Listing up the equipped items slots
	UPROPERTY(BlueprintReadOnly, Category = "Gear")
		AGearBase* LH_Equipped_Gear;
	UPROPERTY(BlueprintReadOnly, Category = "Gear")
		AGearBase* LH_Prev_Equipped_Gear;

	UPROPERTY(BlueprintReadOnly, Category = "Gear")
		AGearBase* RH_Equipped_Gear;
	UPROPERTY(BlueprintReadOnly, Category = "Gear")
		AGearBase* RH_Prev_Equipped_Gear;

	UPROPERTY(BlueprintReadOnly, Category = "Gear")
		AGearBase* Equipped_Sheath;

	UPROPERTY(BlueprintReadOnly, Category = "Gear")
		AGearBase* Equipped_Sheathed_Sword;

	UPROPERTY(BlueprintReadOnly, Category = "Gear")
		AGearBase* Equipped_Sheathed_Shield;

	UPROPERTY(BlueprintReadOnly, Category = "Gear")
		ECombatType CombatTypeEnum;

	UPROPERTY(BlueprintAssignable)
		FOnCombatTypeChanged OnCombatTypeChanged;

	//Set Default Equipment for Player at all times
	void SetDefaultGear();

	//Set Equipment for Player Left Hand
	void Equip_SwordShield1H();

	//Unequip any weapon
	void UnEquip_SwordShield1H();

	//Equip Shield Directly (from Right Trigger Input)
	void Equip_Shield1H();

	//Equip Shield Directly (from Right Trigger Input)
	void UnEquip_Shield1H();

	//Equip Shield Directly (from Right Trigger Input)
	void Equip_Shield_Only();

	//Equip Shield Directly (from Right Trigger Input)
	void UnEquip_Shield_Only();

	//Used to store and alter Previous/Current/New Combat Type
	void SetCombatType(ECombatType NewCombatType);

	//Used to set current LH Gear
	void Set_Current_LH_Equipped_Gear(AGearBase* LH_Gear);

	//Used to set current RH Gear
	void Set_Current_RH_Equipped_Gear(AGearBase* RH_Gear);

	//Used to set current LH Gear
	void UnSet_Current_LH_Equipped_Gear(AGearBase* LH_Gear);

	//Used to set current RH Gear
	void UnSet_Current_RH_Equipped_Gear(AGearBase* RH_Gear);

	//Used to store Equipped State
	UPROPERTY(BlueprintReadOnly, Category = "Gear")
		bool RHGeared = false;

	//Used to store Equipped State
	UPROPERTY(BlueprintReadOnly, Category = "Gear")
		bool LHGeared = false;

	UPROPERTY(BlueprintReadOnly, Category = "Gear")
		ECombatType CurrentCombatType = ECombatType::Unarmed;

	UPROPERTY(BlueprintReadOnly, Category = "Gear")
		ECombatType PrevCombatType;

	UPROPERTY(BlueprintReadOnly, Category = "Gear")
		class UDataTable* GearListDataTable;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	//Sets the Owner to the Player 
	APlayerBase* OwnerPlayer;

	//Attach Parameters (constant for All attachments)
	FActorSpawnParameters Params;
};