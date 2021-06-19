// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/GamepadInputComponent.h"
#include "Gear/Sword1H.h"
#include "Characters/Player/PlayerControllerBase.h"
#include "Characters/Player/PlayerBase.h"


// Sets default values for this component's properties
UGamepadInputComponent::UGamepadInputComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

void UGamepadInputComponent::FaceRight_Button_Implementation()
{
	//Getting the current Axis Value to orient the Action to perform
	FVector AxisValues = CharacterRef->GetPlayerController()->GetAxisValues();
	float xAxisValue = OwnerController->GetAxisValues().X;
	float yAxisValue = OwnerController->GetAxisValues().Y;

	//If we are in classic view
	if (CharacterRef->GetActionsManager()->CurrentViewType == EViewType::Classic)
	{	
		//STANDING STILL = NORMAL ATTACK
		if (xAxisValue == 0 && yAxisValue == 0 && CharacterRef->GetEquipmentManager()->CurrentCombatType != ECombatType::Shield && CharacterRef->GetActionsManager()->CanAttack == true)
		{
			CharacterRef->GetActionsManager()->FaceRight_Button_NormalAttack_Implementation();
		}

		//MOVING = FORWARD ATTACK
		else if (xAxisValue != 0 || yAxisValue != 0 && CharacterRef->GetEquipmentManager()->CurrentCombatType != ECombatType::Shield && CharacterRef->GetActionsManager()->CanAttack == true)
		{
			CharacterRef->GetActionsManager()->FaceRight_Button_ForwardAttack_Implementation();
		}

		//SHIELD ATTACK
		else if (CharacterRef->GetPlayerController()->RightTriggerHeld == true && CharacterRef->GetActionsManager()->CanAttack == true)
		{
			CharacterRef->GetActionsManager()->RightTrigger_Attack_Implementation();
		}
	}
	
	//If we are in Z view
	else if (CharacterRef->GetActionsManager()->CurrentViewType == EViewType::ZView)
	{
		//STANDING STILL = NORMAL ATTACK
		if (xAxisValue == 0 && yAxisValue == 0 && CharacterRef->GetEquipmentManager()->CurrentCombatType != ECombatType::Shield && CharacterRef->GetActionsManager()->CanAttack == true)
		{
			CharacterRef->GetActionsManager()->FaceRight_Button_ZView_NormalAttack_Implementation();
		}

		//MOVING FWD = FORWARD ATTACK
		else if (xAxisValue >= 0 && yAxisValue == 0 && CharacterRef->GetEquipmentManager()->CurrentCombatType != ECombatType::Shield && CharacterRef->GetActionsManager()->CanAttack == true)
		{
			CharacterRef->GetActionsManager()->FaceRight_Button_ZView_ForwardAttack_Implementation();
		}

		//MOVING = SIDE ATTACK
		else if (xAxisValue == 0 && yAxisValue != 0 && CharacterRef->GetEquipmentManager()->CurrentCombatType != ECombatType::Shield && CharacterRef->GetActionsManager()->CanAttack == true)
		{
			CharacterRef->GetActionsManager()->FaceRight_Button_ZView_SideAttack_Implementation();
		}

		//MOVING BCK = BACKWARD ATTACK
		else if (xAxisValue <= 0 && yAxisValue == 0 && CharacterRef->GetEquipmentManager()->CurrentCombatType != ECombatType::Shield && CharacterRef->GetActionsManager()->CanAttack == true)
		{
			CharacterRef->GetActionsManager()->FaceRight_Button_ZView_BackwardAttack_Implementation();
		}
	}

}

void UGamepadInputComponent::FaceRight_Button_Held_Implementation()
{
	if (CharacterRef->GetEquipmentManager()->CurrentCombatType != ECombatType::Shield)
	{
		CharacterRef->GetActionsManager()->FaceRight_Button_ChargedAttack_Implementation();
	}
}

void UGamepadInputComponent::FaceRight_Button_Released_Implementation()
{
	if (CharacterRef->GetEquipmentManager()->CurrentCombatType != ECombatType::Shield)
	{
		CharacterRef->GetActionsManager()->FaceRight_Button_ChargedAttack_Released_Implementation();
	}
}

void UGamepadInputComponent::FaceBottom_Button_Implementation()
{
	//Getting the current Axis Value to orient the Action to perform
	FVector AxisValues = CharacterRef->GetPlayerController()->GetAxisValues();
	float xAxisValue = OwnerController->GetAxisValues().X;
	float yAxisValue = OwnerController->GetAxisValues().Y;

	//STANDING STILL = SHEATH - CLASSIC VIEW
	if (CharacterRef->GetActionsManager()->CurrentViewType == EViewType::Classic && (xAxisValue == 0 && yAxisValue == 0) && CharacterRef->GetEquipmentManager()->CurrentCombatType != ECombatType::Unarmed && CharacterRef->GetEquipmentManager()->CurrentCombatType != ECombatType::Shield)
	{
		CharacterRef->GetActionsManager()->FaceBottom_Button_Sheath_Implementation();
	}

	//MOVING = ROLL - CLASSIC VIEW
	else if (CharacterRef->GetActionsManager()->CurrentViewType == EViewType::Classic && (xAxisValue != 0 || yAxisValue != 0) && CharacterRef->GetEquipmentManager()->CurrentCombatType != ECombatType::Shield)
	{		
		CharacterRef->GetActionsManager()->FaceBottom_Button_Roll_Implementation();
	}

	//MOVING = ROLL - ZVIEW - ONLY IF UNEQUIP
	else if (CharacterRef->GetActionsManager()->CurrentViewType == EViewType::ZView && yAxisValue == 0 && xAxisValue >= 0 && CharacterRef->GetEquipmentManager()->CurrentCombatType == ECombatType::Unarmed)
	{
		CharacterRef->GetActionsManager()->FaceBottom_Button_Roll_Implementation();
	}

	//ZVIEW - JUMP & JUMP ATTACK
	else if (CharacterRef->GetActionsManager()->CurrentViewType == EViewType::ZView && CharacterRef->GetActionsManager()->CanJump == true)
	{
		CharacterRef->GetActionsManager()->FaceBottom_Button_Jump_Implementation(xAxisValue, yAxisValue);
	}

}

void UGamepadInputComponent::RightTrigger_Implementation()
{
	if (CharacterRef->GetEquipmentManager()->CurrentCombatType != ECombatType::Charge && CharacterRef->GetActionsManager()->CurrentViewType != EViewType::ZView)
	{
		//We check what kind of Weapon is the LH Equipped Gear
		ASword1H* SwordBase = Cast<ASword1H>(CharacterRef->GetEquipmentManager()->LH_Equipped_Gear);
		if (SwordBase || SwordBase == NULL)
		{
			//We stop the montage quickly in order to give priority to Shielding
			CharacterRef->GetPlayerAnimInstance()->Montage_Stop(0.1f, CharacterRef->GetPlayerAnimInstance()->GetCurrentActiveMontage());
			//We Equip the Shield and therefore Shielding Stance
			CharacterRef->GetEquipmentManager()->Equip_Shield1H();
		}
	}
	else if (CharacterRef->GetEquipmentManager()->CurrentCombatType != ECombatType::Charge && CharacterRef->GetActionsManager()->CurrentViewType == EViewType::ZView)
	{
		//We stop the montage quickly in order to give priority to Shielding
		CharacterRef->GetPlayerAnimInstance()->Montage_Stop(0.1f, CharacterRef->GetPlayerAnimInstance()->GetCurrentActiveMontage());
		//We Equip the Shield and therefore Shielding Stance
		CharacterRef->GetEquipmentManager()->Equip_Shield_Only();
	}
}

void UGamepadInputComponent::RightTrigger_Released_Implementation()
{
	if (CharacterRef->GetEquipmentManager()->CurrentCombatType != ECombatType::Charge && CharacterRef->GetActionsManager()->CurrentViewType == EViewType::Classic)
	{
		//We check what kind of Weapon is the LH Equipped Gear
		ASword1H* SwordBase = Cast<ASword1H>(CharacterRef->GetEquipmentManager()->LH_Equipped_Gear);
		if (SwordBase)
		{
			//We UnEquip the Shield and therefore back to previous Stance (in this case, always Sword1H)
			CharacterRef->GetEquipmentManager()->UnEquip_Shield1H();
		}
	}
	else if (CharacterRef->GetEquipmentManager()->CurrentCombatType != ECombatType::Charge && CharacterRef->GetActionsManager()->CurrentViewType == EViewType::ZView)
	{
		//We stop the montage quickly in order to give priority to Shielding
		CharacterRef->GetPlayerAnimInstance()->Montage_Stop(0.1f, CharacterRef->GetPlayerAnimInstance()->GetCurrentActiveMontage());
		//We Equip the Shield and therefore Shielding Stance
		CharacterRef->GetEquipmentManager()->UnEquip_Shield_Only();
	}

}

void UGamepadInputComponent::LeftTrigger_Implementation()
{
	if (CharacterRef->GetActionsManager()->CurrentViewType != EViewType::ZView)
	{
		CharacterRef->GetActionsManager()->LeftTrigger_ZView_Implementation();
	}
}

void UGamepadInputComponent::LeftTrigger_Released_Implementation()
{
	if (CharacterRef->GetActionsManager()->CurrentViewType == EViewType::ZView)
	{
		CharacterRef->GetActionsManager()->LeftTrigger_ClassicView_Implementation();
	}
}

// Called when the game starts
void UGamepadInputComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerController = Cast<APlayerControllerBase>(this->GetOwner());
	CharacterRef = OwnerController->GetPossessedPawn();
	
}


// Called every frame
void UGamepadInputComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

