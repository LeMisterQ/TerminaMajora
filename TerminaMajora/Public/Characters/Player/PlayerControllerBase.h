// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Components/GamepadInputComponent.h"
#include "Characters/Player/PlayerBase.h"
#include "UI/HUDBase.h"
#include "PlayerControllerBase.generated.h"

//Forward Declaration
class UAnimInstanceBase;

UCLASS()
class TERMINAMAJORA_API APlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UGamepadInputComponent* GamepadInputComponent;

	
public:

	//Constructor
	APlayerControllerBase();

	// Called to bind functionality to input
	virtual void SetupInputComponent() override;

	//Storing Character Reference
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reference")
		class APlayerBase* CharacterRef;

	//Getter for the possessed Pawn
	APlayerBase* GetPossessedPawn();

	//Input Mapping functions
	//Axis Mapping
	void MoveForward(float Value);
	void MoveRight(float Value);
	void MoveUp(float Value);
	void Turn(float Value);
	void TurnRate(float Value);
	void LookUp(float Value);
	void LookUpRate(float Value);

	//Action Mapping
	//BlueprintNative allows us to use both Code AND Blueprint after code to do stuff
	UFUNCTION(BlueprintNativeEvent, Category = "Input")
		void FaceRight_Button();
	virtual void FaceRight_Button_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Input")
	void FaceRight_Button_Released();
	virtual void FaceRight_Button_Released_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Input")
		void FaceRight_Button_Held();
	virtual void FaceRight_Button_Held_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Input")
		void FaceBottom_Button();
	virtual void FaceBottom_Button_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Input")
		void RightTrigger();
	virtual void RightTrigger_Implementation();
	
	UFUNCTION(BlueprintNativeEvent, Category = "Input")
		void RightTrigger_Released();
	virtual void RightTrigger_Released_Implementation();

	//Timer to check if we released Shield
	UFUNCTION(BlueprintNativeEvent, Category = "Input")
		void RightTrigger_Held();
	virtual void RightTrigger_Held_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Input")
		void LeftTrigger();
	virtual void LeftTrigger_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Input")
		void LeftTrigger_Released();
	virtual void LeftTrigger_Released_Implementation();

	//Timer to check if we released Z View
	UFUNCTION(BlueprintNativeEvent, Category = "Input")
		void LeftTrigger_Held();
	virtual void LeftTrigger_Held_Implementation();
	
	//Expose a component to Blueprint (always this setup for that purpose)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float BaseTurnRate = 45.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float BaseLookAtRate = 45.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		float FwdInputAxisValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		float RghtInputAxisValue;

	//Returns the axis values of the MoveForward and MoveRight axis
	UFUNCTION()
	FVector GetAxisValues();

	//Used to check if button is held
	bool RightJoystick_XAxis_Used = false;
	bool RightJoystick_YAxis_Used = false;
	bool RightTriggerHeld = false;
	bool LeftTriggerHeld = false;
	bool FaceRight_ButtonHeld = false;

	//HUD reference
	class AHUDBase* HUDBase;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	//CustomTimerHandle
	FTimerHandle FaceRight_Button_Held_Check;
	FTimerHandle LeftTrigger_Held_Check;
	FTimerHandle RightTrigger_Held_Check;

	//Movement variables
	FRotator YawRotation = FRotator::ZeroRotator;

	//Time to wait before activating the Charged Attack stance
	float ChargedAttackTimer = 0.50f;
	
};