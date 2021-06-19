// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/PlayerControllerBase.h"
#include "TimerManager.h"
#include "UI/HUDBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "AnimInstance/AnimInstanceBase.h"


APlayerControllerBase::APlayerControllerBase()
{
	//Create and Attach GamepadInput Component
	GamepadInputComponent = CreateDefaultSubobject<UGamepadInputComponent>(TEXT("GamepadInputComponent"));
	
}

void APlayerControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();

	//Handles the binding of input to actions/axis

	InputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerControllerBase::MoveForward);
	InputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerControllerBase::MoveRight);
	InputComponent->BindAxis(TEXT("Turn"), this, &APlayerControllerBase::Turn);
	InputComponent->BindAxis(TEXT("TurnRate"), this, &APlayerControllerBase::TurnRate);
	InputComponent->BindAxis(TEXT("LookUp"), this, &APlayerControllerBase::LookUp);
	InputComponent->BindAxis(TEXT("LookUpRate"), this, &APlayerControllerBase::LookUpRate);

	InputComponent->BindAction(TEXT("FaceRight_Button"), IE_Pressed, this, &APlayerControllerBase::FaceRight_Button);
	InputComponent->BindAction(TEXT("FaceRight_Button"), IE_Released, this, &APlayerControllerBase::FaceRight_Button_Released);
	InputComponent->BindAction(TEXT("FaceBottom_Button"), IE_Pressed, this, &APlayerControllerBase::FaceBottom_Button);
	InputComponent->BindAction(TEXT("RightTrigger"), IE_Pressed, this, &APlayerControllerBase::RightTrigger);
	InputComponent->BindAction(TEXT("RightTrigger"), IE_Released, this, &APlayerControllerBase::RightTrigger_Released);
	InputComponent->BindAction(TEXT("LeftTrigger"), IE_Pressed, this, &APlayerControllerBase::LeftTrigger);
	InputComponent->BindAction(TEXT("LeftTrigger"), IE_Released, this, &APlayerControllerBase::LeftTrigger_Released);
}

APlayerBase* APlayerControllerBase::GetPossessedPawn()
{
	//Get the reference to the possessed Character when Game starts (does not work in constructor obviously)
	CharacterRef = Cast<APlayerBase>(this->GetPawn());

	return CharacterRef;
}

// Sets up input functionalities
void APlayerControllerBase::MoveForward(float Value)
{
	if (Value == 0)
	{
		RightJoystick_YAxis_Used = false;
	}
	else
	{
		RightJoystick_YAxis_Used = true;
	}	

	//Store the value in a variable for future use
	FwdInputAxisValue = InputComponent->GetAxisValue(TEXT("MoveForward"));
	//Sets up the Rotator for the Forward Movement capabilities (3rd Person)
	float ZRotation = this->GetControlRotation().Yaw;
	YawRotation = UKismetMathLibrary::MakeRotator(0.0, 0.0, ZRotation);
	CharacterRef->AddMovementInput(UKismetMathLibrary::GetForwardVector(YawRotation), Value, false);

}

void APlayerControllerBase::MoveRight(float Value)
{
	if (Value == 0)
	{
		RightJoystick_XAxis_Used = false;
	}
	else
	{
		RightJoystick_XAxis_Used = true;
	}
	
	//Store the value in a variable for future use
	RghtInputAxisValue = InputComponent->GetAxisValue(TEXT("MoveRight"));
	//Sets up the Rotator for the Right Movement capabilities (3rd Person)
	float ZRotation = this->GetControlRotation().Yaw;
	YawRotation = UKismetMathLibrary::MakeRotator(0.0, 0.0, ZRotation);
	CharacterRef->AddMovementInput(UKismetMathLibrary::GetRightVector(YawRotation), Value, false);

}

void APlayerControllerBase::Turn(float Value)
{
	CharacterRef->AddControllerYawInput(Value);
}

void APlayerControllerBase::TurnRate(float Value)
{
	if (GetPossessedPawn())
	{
		//If We are not in ZView
		if (GetPossessedPawn()->GetActionsManager()->CurrentViewType == EViewType::Classic)
		{
			//We make it frame-independent by using the Delta (delta-time between frames)
			CharacterRef->AddControllerYawInput(Value * BaseTurnRate * GetWorld()->GetDeltaSeconds());
		}
	}
}

void APlayerControllerBase::LookUp(float Value)
{
	CharacterRef->AddControllerPitchInput(Value);
}

void APlayerControllerBase::LookUpRate(float Value)
{
	if (GetPossessedPawn())
	{
		//If We are not in ZView
		if (GetPossessedPawn()->GetActionsManager()->CurrentViewType == EViewType::Classic)
		{
			//We make it frame-independent by using the Delta (delta-time between frames)
			CharacterRef->AddControllerPitchInput(Value * BaseLookAtRate * GetWorld()->GetDeltaSeconds());
		}
	}
}

void APlayerControllerBase::FaceRight_Button_Implementation()
{
	//Start A Timer to check if Button is held -> See FaceRight_Button_Held_Implementation
	GetWorldTimerManager().SetTimer(FaceRight_Button_Held_Check, this, &APlayerControllerBase::FaceRight_Button_Held, 0.016f, true, 0.016f);
	GamepadInputComponent->FaceRight_Button_Implementation();
}

void APlayerControllerBase::FaceRight_Button_Released_Implementation()
{
	if(FaceRight_ButtonHeld == true)
	{ 
		//We clear the timer and reset the bool for Face Right Button Hold
		FaceRight_ButtonHeld = false;
		GetWorldTimerManager().ClearTimer(FaceRight_Button_Held_Check);
		GamepadInputComponent->FaceRight_Button_Released_Implementation();
		
	}
}

void APlayerControllerBase::FaceRight_Button_Held_Implementation()
{
	if (this->GetInputKeyTimeDown("Gamepad_FaceButton_Right") >= ChargedAttackTimer)
	{
		FaceRight_ButtonHeld = true;
		GamepadInputComponent->FaceRight_Button_Held_Implementation();
	}
}

void APlayerControllerBase::FaceBottom_Button_Implementation()
{
	GamepadInputComponent->FaceBottom_Button_Implementation();
}

void APlayerControllerBase::RightTrigger_Implementation()
{	
	// If we are not already Shielding, we can do it
	if (GetPossessedPawn()->GetEquipmentManager()->CurrentCombatType != ECombatType::Shield)
	{				
		GamepadInputComponent->RightTrigger_Implementation();
		GetWorldTimerManager().SetTimer(RightTrigger_Held_Check, this, &APlayerControllerBase::RightTrigger_Held, 0.016f, true, 0.016f);
	}
}

void APlayerControllerBase::RightTrigger_Released_Implementation()
{
	if (RightTriggerHeld == false)
	{
		GetWorldTimerManager().ClearTimer(RightTrigger_Held_Check);
		GamepadInputComponent->RightTrigger_Released_Implementation();
	}
}

void APlayerControllerBase::RightTrigger_Held_Implementation()
{

	if (this->GetInputKeyTimeDown("Gamepad_RightTrigger") > 0.0f)
	{
		RightTriggerHeld = true;
	}
	else
	{
		RightTriggerHeld = false;
		RightTrigger_Released();
	}
}

void APlayerControllerBase::LeftTrigger_Implementation()
{
	// If we are not already in ZView, we can enter it
	if (GetPossessedPawn()->GetActionsManager()->CurrentViewType != EViewType::ZView)
	{
		//If HUDBase is not NULLPTR
		if (HUDBase)
		{
			HUDBase->GoIntoZView();
			GamepadInputComponent->LeftTrigger();
			GetWorldTimerManager().SetTimer(LeftTrigger_Held_Check, this, &APlayerControllerBase::LeftTrigger_Held, 0.016f, true, 0.016f);
		}
	}
}

void APlayerControllerBase::LeftTrigger_Released_Implementation()
{
	if (LeftTriggerHeld == false)
	{
		//If HUDBase is not NULLPTR
		if (HUDBase)
		{			
			UAnimInstanceBase* CustomAnimInstance = Cast<UAnimInstanceBase>(GetPossessedPawn()->GetPlayerAnimInstance());
			if (CustomAnimInstance->CanExitZView)
			{				

				GetWorldTimerManager().ClearTimer(LeftTrigger_Held_Check);
				HUDBase->GoIntoClassicView();
				GamepadInputComponent->LeftTrigger_Released();
			}

		}
	}
}

void APlayerControllerBase::LeftTrigger_Held_Implementation()
{
	if (this->GetInputKeyTimeDown("Gamepad_LeftTrigger") > 0.0f)
	{
		LeftTriggerHeld = true;
	}
	else
	{
		LeftTriggerHeld = false;
		LeftTrigger_Released();
	}
}

FVector APlayerControllerBase::GetAxisValues()
{
	return FVector(
		InputComponent->GetAxisValue(TEXT("MoveForward")),
		InputComponent->GetAxisValue(TEXT("MoveRight")),
		0);
}

// Called when the game starts or when spawned
void APlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();	

	//Standard process for obtaining the runtime HUD
	HUDBase = Cast<AHUDBase>(GetWorld()->GetFirstPlayerController()->GetHUD());

}