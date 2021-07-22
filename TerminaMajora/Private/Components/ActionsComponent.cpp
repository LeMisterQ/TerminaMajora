// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ActionsComponent.h"
#include "Characters/Player/PlayerBase.h"
#include "AnimInstance/AnimInstanceBase.h"
#include "Gear/Sword1H.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

// Sets default values for this component's properties
UActionsComponent::UActionsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ActionsMontagesDataTable = GetActionsMontagesDataTable();

	//Attach LineTrace for ledge detection
	LineTrace = CreateDefaultSubobject<ULineTraceBase>(TEXT("LineTrace"));

}


// Called when the game starts
void UActionsComponent::BeginPlay()
{
	Super::BeginPlay();

	//Declaring OwnerPlayer firstly
	OwnerPlayer = Cast<APlayerBase>(this->GetOwner());
	ActionsMontagesDataTable = GetActionsMontagesDataTable();
	EquipmentManager = OwnerPlayer->GetEquipmentManager();
	PlayerAnimInstance = OwnerPlayer->GetPlayerAnimInstance();

	//ZView Management - Setting all useful information
	SpringArm_TargetArmLength = OwnerPlayer->GetSpringArmComponent()->TargetArmLength;
	SpringArm_ZOffset = OwnerPlayer->GetSpringArmComponent()->SocketOffset.Z;
	Controller_Initial_Rotation = OwnerPlayer->GetCameraOrientRef()->GetComponentRotation();

	//ZView camera transition Management
	if (ZViewCurveFloat)
	{
		//Initializing the TimeLine
		FOnTimelineFloat ZViewProgress;

		//Initializing the Curve onto the first Timeline
		ZViewProgress.BindUFunction(this, FName("ZViewSpringArmUpdate"));
		ZViewCameraFTimeline.AddInterpFloat(ZViewCurveFloat, ZViewProgress);

		//Initializing the Curve onto the second Timeline
		ZViewProgress.BindUFunction(this, FName("ZViewCameraInBack"));
		ZViewCameraInBackFTimeline.AddInterpFloat(ZViewCurveFloat, ZViewProgress);

		//Initializing the Curve onto the third Timeline
		ZViewProgress.BindUFunction(this, FName("ZViewRotateYCamera"));
		ZViewRotateYCameraFTimeline.AddInterpFloat(ZViewCurveFloat, ZViewProgress);

	}
	
}

// Called every frame
void UActionsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// !IMPORTANT : Timeline setting delta time
	ZViewCameraFTimeline.TickTimeline(DeltaTime);
	ZViewCameraInBackFTimeline.TickTimeline(DeltaTime);
	ZViewRotateYCameraFTimeline.TickTimeline(DeltaTime);

	//Ledge Detection for Auto Jump : if no floor detected when tracing at least the Max Step Height : ledge detected and auto-jump possible
	//LineTrace parameters
	FCollisionQueryParams TraceParams;
	TraceParams.bTraceComplex = false;
	float CapsuleRadius = OwnerPlayer->GetCapsuleComponent()->GetScaledCapsuleRadius();
	float CapsuleHalfHeight = OwnerPlayer->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	float CurrentSpeed = OwnerPlayer->GetCustomAnimInstance()->Speed;
	float CurrentMaxSpeed = OwnerPlayer->GetMovementComp()->MaxWalkSpeed * 0.75f;
	FString ResultFloor = LineTrace->FloorTypeCheck(OwnerPlayer, FName("foot_r"));

	TraceStart = FVector(OwnerPlayer->GetActorLocation().X, OwnerPlayer->GetActorLocation().Y, (OwnerPlayer->GetActorLocation().Z - CapsuleHalfHeight)) + FVector(OwnerPlayer->GetActorForwardVector() * CapsuleRadius);
	TraceEnd = FVector(TraceStart.X, TraceStart.Y, TraceStart.Z - OwnerPlayer->GetMovementComp()->MaxStepHeight);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams);
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, TraceDuration);

	if (bHit)
	{
		/*DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5), FColor::Emerald, false, TraceDuration);*/
	}
	//If we detect a ledge + player is running, THEN we jump
	else if (!bHit && CurrentSpeed >= CurrentMaxSpeed && (!ResultFloor.IsEmpty() && !ResultFloor.Contains("no_hit") && !ResultFloor.Contains("landscape")))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, ResultFloor);
		ExecJump(EJumpTypes::StandardJump);
	}
}

void UActionsComponent::FaceRight_Button_NormalAttack_Implementation()
{
	//EQUIP
	if (EquipmentManager->CurrentCombatType == ECombatType::Unarmed && PlayerAnimInstance->IsAnyMontagePlaying() == false)
	{
		/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Equip and Normal Attack !"));*/

		if (ActionsMontagesDataTable)
		{
			//Getting montages data from DataTable Row
			FActionsMontages* MontagesStruct = ActionsMontagesDataTable->FindRow<FActionsMontages>(FName(TEXT("UNARMED")), CustomContextString, true);

			//Fetch the corresponding CharacterActions Struct (1st level)
			TMap<ECharacterActions, FAnimMontages> CharacterActions = MontagesStruct->CharacterActions;
			//Get the Montage struct corresponding to the enum we want
			FAnimMontages EquipAnimMontageStruct = CharacterActions.FindRef(ECharacterActions::EquipWeaponNormal);
			//Get the Montage from the last Montages Struct
			UAnimMontage* FetchedMontage = EquipAnimMontageStruct.AnimMontage;

			//Plays the montage
			OwnerPlayer->PlayAnimMontage(FetchedMontage, 1.0f);
		}

	}

	//NORMAL ATTACK COMBO
	else if (EquipmentManager->CurrentCombatType == ECombatType::Sword1H)
	{

		/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("NormalAttack !"));*/

		if (ActionsMontagesDataTable)
		{
			//UNLOCKING ATTACK ABILITIES
			if (CanAttack == true)
			{
				//Deactivating Attack during the montage (unless authorized later on in cases of combo)
				CanAttack = false;

				//Getting montages data from DataTable Row
				FActionsMontages* MontagesStruct = ActionsMontagesDataTable->FindRow<FActionsMontages>(FName(TEXT("SWORD1H")), CustomContextString, true);

				//Fetch the corresponding CharacterActions Struct (1st level)
				TMap<EAttackTypes, FComboAnimMontages> Attacks = MontagesStruct->Attacks;
				//Get the Montage struct corresponding to the enum we want
				FComboAnimMontages AttacksMontageStruct = Attacks.FindRef(EAttackTypes::NormalAttack);
				//Get the Montage from the last Montages Struct
				TArray<FAnimMontages> MontagesArray = AttacksMontageStruct.ComboAnimMontage;
				//Size of the montages Array
				AttackIndexMax = (MontagesArray.Num() - 1);
				//Get the Montage Struct based on current Index
				FAnimMontages FetchedMontageStruct = MontagesArray[AttackIndexCurrent];
				UAnimMontage* FetchedMontage = FetchedMontageStruct.AnimMontage;			

				//Plays the montage
				OwnerPlayer->PlayAnimMontage(FetchedMontage, 1.0f);

				//Updates the Attack Index Current
				if (AttackIndexCurrent < AttackIndexMax)
				{
					//Incrementing Montage Sequence
					AttackIndexCurrent++;
				}
				else
				{
					//Reseting Montage Sequence
					AttackIndexCurrent = 0;

				}
			}
			
		}
	}

}

void UActionsComponent::FaceRight_Button_ZView_NormalAttack_Implementation()
{
	//EQUIP
	if (EquipmentManager->CurrentCombatType == ECombatType::Unarmed && PlayerAnimInstance->IsAnyMontagePlaying() == false)
	{
		/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("ZView Equip and Normal Attack !"));*/

		if (ActionsMontagesDataTable)
		{
			//Getting montages data from DataTable Row
			FActionsMontages* MontagesStruct = ActionsMontagesDataTable->FindRow<FActionsMontages>(FName(TEXT("UNARMED")), CustomContextString, true);

			//Fetch the corresponding CharacterActions Struct (1st level)
			TMap<ECharacterActions, FAnimMontages> CharacterActions = MontagesStruct->CharacterActions;
			//Get the Montage struct corresponding to the enum we want
			FAnimMontages EquipAnimMontageStruct = CharacterActions.FindRef(ECharacterActions::EquipWeaponNormal);
			//Get the Montage from the last Montages Struct
			UAnimMontage* FetchedMontage = EquipAnimMontageStruct.AnimMontage;

			//Plays the montage
			OwnerPlayer->PlayAnimMontage(FetchedMontage, 1.0f);
		}

	}

	//NORMAL ATTACK COMBO
	else if (EquipmentManager->CurrentCombatType == ECombatType::Sword1H)
	{
		/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Z View NormalAttack !"));*/

		if (ActionsMontagesDataTable)
		{
			//UNLOCKING ATTACK ABILITIES
			if (CanAttack == true)
			{
				//Deactivating Attack during the montage (unless authorized later on in cases of combo)
				CanAttack = false;

				//Getting montages data from DataTable Row
				FActionsMontages* MontagesStruct = ActionsMontagesDataTable->FindRow<FActionsMontages>(FName(TEXT("SWORD1H")), CustomContextString, true);

				//Fetch the corresponding CharacterActions Struct (1st level)
				TMap<EAttackTypes, FComboAnimMontages> Attacks = MontagesStruct->Attacks;
				//Get the Montage struct corresponding to the enum we want
				FComboAnimMontages AttacksMontageStruct = Attacks.FindRef(EAttackTypes::ZViewNormalAttack);
				//Get the Montage from the last Montages Struct
				TArray<FAnimMontages> MontagesArray = AttacksMontageStruct.ComboAnimMontage;
				//Size of the montages Array
				AttackIndexMax = (MontagesArray.Num() - 1);
				//Get the Montage Struct based on current Index
				FAnimMontages FetchedMontageStruct = MontagesArray[AttackIndexCurrent];
				UAnimMontage* FetchedMontage = FetchedMontageStruct.AnimMontage;

				//Plays the montage
				OwnerPlayer->PlayAnimMontage(FetchedMontage, 1.0f);

				//Updates the Attack Index Current
				if (AttackIndexCurrent < AttackIndexMax)
				{
					//Incrementing Montage Sequence
					AttackIndexCurrent++;
				}
				else
				{
					//Reseting Montage Sequence
					AttackIndexCurrent = 0;

				}
			}

		}
	}
}

void UActionsComponent::FaceRight_Button_ForwardAttack_Implementation()
{
	//EQUIP
	if (EquipmentManager->CurrentCombatType == ECombatType::Unarmed && PlayerAnimInstance->IsAnyMontagePlaying() == false)
	{
		/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Equip and Forward Attack !"));*/

		if (ActionsMontagesDataTable)
		{	
			//Getting montages data from DataTable Row
			FActionsMontages* MontagesStruct = ActionsMontagesDataTable->FindRow<FActionsMontages>(FName(TEXT("UNARMED")), CustomContextString, true);

			//Fetch the corresponding CharacterActions Struct (1st level)
			TMap<ECharacterActions, FAnimMontages> CharacterActions = MontagesStruct->CharacterActions;
			//Get the Montage struct corresponding to the enum we want
			FAnimMontages EquipAnimMontageStruct = CharacterActions.FindRef(ECharacterActions::EquipWeaponForward);
			//Get the Montage from the last Montages Struct
			UAnimMontage* FetchedMontage = EquipAnimMontageStruct.AnimMontage;

			//Plays the montage
			OwnerPlayer->PlayAnimMontage(FetchedMontage, 1.0f);
		}
	}

	//FORWARD ATTACK COMBO
	else if (EquipmentManager->CurrentCombatType == ECombatType::Sword1H)
	{

		/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("ForwardAttack !"));*/

		if (ActionsMontagesDataTable)
		{
			//UNLOCKING ATTACK ABILITIES
			if (CanAttack == true)
			{
				//Deactivating Attack during the montage (unless authorized later on in cases of combo)
				CanAttack = false;

				//Getting montages data from DataTable Row
				FActionsMontages* MontagesStruct = ActionsMontagesDataTable->FindRow<FActionsMontages>(FName(TEXT("SWORD1H")), CustomContextString, true);

				//Fetch the corresponding CharacterActions Struct (1st level)
				TMap<EAttackTypes, FComboAnimMontages> Attacks = MontagesStruct->Attacks;
				//Get the Montage struct corresponding to the enum we want
				FComboAnimMontages AttacksMontageStruct = Attacks.FindRef(EAttackTypes::ForwardAttack);
				//Get the Montage from the last Montages Struct
				TArray<FAnimMontages> MontagesArray = AttacksMontageStruct.ComboAnimMontage;
				//Size of the montages Array
				AttackIndexMax = (MontagesArray.Num() - 1);
				//Get the Montage Struct based on current Index
				FAnimMontages FetchedMontageStruct = MontagesArray[AttackIndexCurrent];
				UAnimMontage* FetchedMontage = FetchedMontageStruct.AnimMontage;

				//Plays the montage
				OwnerPlayer->PlayAnimMontage(FetchedMontage, 1.0f);

				//Updates the Attack Index Current
				if (AttackIndexCurrent < AttackIndexMax)
				{
					//Incrementing Montage Sequence
					AttackIndexCurrent++;
				}
				else
				{
					//Reseting Montage Sequence
					AttackIndexCurrent = 0;

				}
			}

		}
	}

}

void UActionsComponent::FaceRight_Button_ZView_ForwardAttack_Implementation()
{
	//EQUIP
	if (EquipmentManager->CurrentCombatType == ECombatType::Unarmed && PlayerAnimInstance->IsAnyMontagePlaying() == false)
	{
		/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Z View Equip and Forward Attack !"));*/

		if (ActionsMontagesDataTable)
		{
			//Getting montages data from DataTable Row
			FActionsMontages* MontagesStruct = ActionsMontagesDataTable->FindRow<FActionsMontages>(FName(TEXT("UNARMED")), CustomContextString, true);

			//Fetch the corresponding CharacterActions Struct (1st level)
			TMap<ECharacterActions, FAnimMontages> CharacterActions = MontagesStruct->CharacterActions;
			//Get the Montage struct corresponding to the enum we want
			FAnimMontages EquipAnimMontageStruct = CharacterActions.FindRef(ECharacterActions::EquipWeaponForward);
			//Get the Montage from the last Montages Struct
			UAnimMontage* FetchedMontage = EquipAnimMontageStruct.AnimMontage;

			//Plays the montage
			OwnerPlayer->PlayAnimMontage(FetchedMontage, 1.0f);
		}
	}

	//FORWARD ATTACK COMBO
	else if (EquipmentManager->CurrentCombatType == ECombatType::Sword1H)
	{

		/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Z View ForwardAttack !"));*/

		if (ActionsMontagesDataTable)
		{
			//UNLOCKING ATTACK ABILITIES
			if (CanAttack == true)
			{
				//Deactivating Attack during the montage (unless authorized later on in cases of combo)
				CanAttack = false;

				//Getting montages data from DataTable Row
				FActionsMontages* MontagesStruct = ActionsMontagesDataTable->FindRow<FActionsMontages>(FName(TEXT("SWORD1H")), CustomContextString, true);

				//Fetch the corresponding CharacterActions Struct (1st level)
				TMap<EAttackTypes, FComboAnimMontages> Attacks = MontagesStruct->Attacks;
				//Get the Montage struct corresponding to the enum we want
				FComboAnimMontages AttacksMontageStruct = Attacks.FindRef(EAttackTypes::ZViewForwardAttack);
				//Get the Montage from the last Montages Struct
				TArray<FAnimMontages> MontagesArray = AttacksMontageStruct.ComboAnimMontage;
				//Size of the montages Array
				AttackIndexMax = (MontagesArray.Num() - 1);
				//Get the Montage Struct based on current Index
				FAnimMontages FetchedMontageStruct = MontagesArray[AttackIndexCurrent];
				UAnimMontage* FetchedMontage = FetchedMontageStruct.AnimMontage;

				//Plays the montage
				OwnerPlayer->PlayAnimMontage(FetchedMontage, 1.0f);

				//Updates the Attack Index Current
				if (AttackIndexCurrent < AttackIndexMax)
				{
					//Incrementing Montage Sequence
					AttackIndexCurrent++;
				}
				else
				{
					//Reseting Montage Sequence
					AttackIndexCurrent = 0;

				}
			}

		}
	}
}

void UActionsComponent::FaceRight_Button_ZView_SideAttack_Implementation()
{
	//EQUIP
	if (EquipmentManager->CurrentCombatType == ECombatType::Unarmed && PlayerAnimInstance->IsAnyMontagePlaying() == false)
	{
		/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("ZView Equip and Normal Attack !"));*/

		if (ActionsMontagesDataTable)
		{
			//Getting montages data from DataTable Row
			FActionsMontages* MontagesStruct = ActionsMontagesDataTable->FindRow<FActionsMontages>(FName(TEXT("UNARMED")), CustomContextString, true);

			//Fetch the corresponding CharacterActions Struct (1st level)
			TMap<ECharacterActions, FAnimMontages> CharacterActions = MontagesStruct->CharacterActions;
			//Get the Montage struct corresponding to the enum we want
			FAnimMontages EquipAnimMontageStruct = CharacterActions.FindRef(ECharacterActions::EquipWeaponNormal);
			//Get the Montage from the last Montages Struct
			UAnimMontage* FetchedMontage = EquipAnimMontageStruct.AnimMontage;

			//Plays the montage
			OwnerPlayer->PlayAnimMontage(FetchedMontage, 1.0f);
		}

	}

	//NORMAL ATTACK COMBO
	else if (EquipmentManager->CurrentCombatType == ECombatType::Sword1H)
	{
		/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Z View SideAttack !"));*/

		if (ActionsMontagesDataTable)
		{
			//UNLOCKING ATTACK ABILITIES
			if (CanAttack == true)
			{
				//Deactivating Attack during the montage (unless authorized later on in cases of combo)
				CanAttack = false;

				//Getting montages data from DataTable Row
				FActionsMontages* MontagesStruct = ActionsMontagesDataTable->FindRow<FActionsMontages>(FName(TEXT("SWORD1H")), CustomContextString, true);

				//Fetch the corresponding CharacterActions Struct (1st level)
				TMap<EAttackTypes, FComboAnimMontages> Attacks = MontagesStruct->Attacks;
				//Get the Montage struct corresponding to the enum we want
				FComboAnimMontages AttacksMontageStruct = Attacks.FindRef(EAttackTypes::ZViewSideAttack);
				//Get the Montage from the last Montages Struct
				TArray<FAnimMontages> MontagesArray = AttacksMontageStruct.ComboAnimMontage;
				//Size of the montages Array
				AttackIndexMax = (MontagesArray.Num() - 1);
				//Get the Montage Struct based on current Index
				FAnimMontages FetchedMontageStruct = MontagesArray[AttackIndexCurrent];
				UAnimMontage* FetchedMontage = FetchedMontageStruct.AnimMontage;

				//Plays the montage
				OwnerPlayer->PlayAnimMontage(FetchedMontage, 1.0f);

				//Updates the Attack Index Current
				if (AttackIndexCurrent < AttackIndexMax)
				{
					//Incrementing Montage Sequence
					AttackIndexCurrent++;
				}
				else
				{
					//Reseting Montage Sequence
					AttackIndexCurrent = 0;

				}
			}

		}
	}
}

void UActionsComponent::FaceRight_Button_ZView_BackwardAttack_Implementation()
{
	//EQUIP
	if (EquipmentManager->CurrentCombatType == ECombatType::Unarmed && PlayerAnimInstance->IsAnyMontagePlaying() == false)
	{
		/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("ZView Equip and Normal Attack !"));*/

		if (ActionsMontagesDataTable)
		{
			//Getting montages data from DataTable Row
			FActionsMontages* MontagesStruct = ActionsMontagesDataTable->FindRow<FActionsMontages>(FName(TEXT("UNARMED")), CustomContextString, true);

			//Fetch the corresponding CharacterActions Struct (1st level)
			TMap<ECharacterActions, FAnimMontages> CharacterActions = MontagesStruct->CharacterActions;
			//Get the Montage struct corresponding to the enum we want
			FAnimMontages EquipAnimMontageStruct = CharacterActions.FindRef(ECharacterActions::EquipWeaponForward);
			//Get the Montage from the last Montages Struct
			UAnimMontage* FetchedMontage = EquipAnimMontageStruct.AnimMontage;

			//Plays the montage
			OwnerPlayer->PlayAnimMontage(FetchedMontage, 1.0f);
		}

	}

	//NORMAL ATTACK COMBO
	else if (EquipmentManager->CurrentCombatType == ECombatType::Sword1H)
	{
		/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Z View SideAttack !"));*/

		if (ActionsMontagesDataTable)
		{
			//UNLOCKING ATTACK ABILITIES
			if (CanAttack == true)
			{
				//Deactivating Attack during the montage (unless authorized later on in cases of combo)
				CanAttack = false;

				//Getting montages data from DataTable Row
				FActionsMontages* MontagesStruct = ActionsMontagesDataTable->FindRow<FActionsMontages>(FName(TEXT("SWORD1H")), CustomContextString, true);

				//Fetch the corresponding CharacterActions Struct (1st level)
				TMap<EAttackTypes, FComboAnimMontages> Attacks = MontagesStruct->Attacks;
				//Get the Montage struct corresponding to the enum we want
				FComboAnimMontages AttacksMontageStruct = Attacks.FindRef(EAttackTypes::NormalAttack);
				//Get the Montage from the last Montages Struct
				TArray<FAnimMontages> MontagesArray = AttacksMontageStruct.ComboAnimMontage;
				//Size of the montages Array
				AttackIndexMax = (MontagesArray.Num() - 1);
				//Get the Montage Struct based on current Index
				FAnimMontages FetchedMontageStruct = MontagesArray[AttackIndexCurrent];
				UAnimMontage* FetchedMontage = FetchedMontageStruct.AnimMontage;

				//Plays the montage
				OwnerPlayer->PlayAnimMontage(FetchedMontage, 1.0f);

				//Updates the Attack Index Current
				if (AttackIndexCurrent < AttackIndexMax)
				{
					//Incrementing Montage Sequence
					AttackIndexCurrent++;
				}
				else
				{
					//Reseting Montage Sequence
					AttackIndexCurrent = 0;

				}
			}

		}
	}
}

void UActionsComponent::FaceRight_Button_ChargedAttack_Implementation()
{
	if ((EquipmentManager->CurrentCombatType != ECombatType::Unarmed || EquipmentManager->CurrentCombatType != ECombatType::Shield))
	{
		/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, TEXT("Charging....."));*/

		//Sets the new Combat Type (Event Dispatcher)
		EquipmentManager->SetCombatType(ECombatType::Charge);

		//Altering the Movement Component to match our needs
		OwnerPlayer->GetMovementComp()->bOrientRotationToMovement = false;
		OwnerPlayer->GetMovementComp()->MaxWalkSpeed = 50.0f;

		//We set CanAttack = True in the Action Manager (since it's not determined by any AnimMontage)
		OwnerPlayer->GetActionsManager()->ResetCanAttack();

	}
}

void UActionsComponent::FaceRight_Button_ChargedAttack_Released_Implementation()
{
	if (EquipmentManager->CurrentCombatType == ECombatType::Charge)
	{
		/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, TEXT("Charging Attack !!!"));*/

		if (ActionsMontagesDataTable)
		{
			//UNLOCKING ATTACK ABILITIES
			if (CanAttack == true)
			{
				//Deactivating Attack during the montage (unless authorized later on in cases of combo)
				CanAttack = false;				

				//Getting montages data from DataTable Row
				FActionsMontages* MontagesStruct = ActionsMontagesDataTable->FindRow<FActionsMontages>(FName(TEXT("SWORD1H")), CustomContextString, true);

				//Fetch the corresponding CharacterActions Struct (1st level)
				TMap<EAttackTypes, FComboAnimMontages> Attacks = MontagesStruct->Attacks;
				//Get the Montage struct corresponding to the enum we want
				FComboAnimMontages AttacksMontageStruct = Attacks.FindRef(EAttackTypes::ChargedAttack);
				//Get the Montage from the last Montages Struct
				TArray<FAnimMontages> MontagesArray = AttacksMontageStruct.ComboAnimMontage;
				//Size of the montages Array
				AttackIndexMax = (MontagesArray.Num() - 1);
				//Get the Montage Struct based on current Index
				FAnimMontages FetchedMontageStruct = MontagesArray[AttackIndexCurrent];
				UAnimMontage* FetchedMontage = FetchedMontageStruct.AnimMontage;

				//Plays the montage
				OwnerPlayer->PlayAnimMontage(FetchedMontage, 1.0f);

				//Altering Back the Movement Component to match our needs
				OwnerPlayer->GetMovementComp()->bOrientRotationToMovement = true;
				OwnerPlayer->GetMovementComp()->MaxWalkSpeed = OwnerPlayer->GetParentClass()->DefaultWalkSpeed;

				//Sets the new Combat Type (Event Dispatcher)
				EquipmentManager->SetCombatType(EquipmentManager->PrevCombatType);

			}
		}
	}
}

void UActionsComponent::FaceBottom_Button_Roll_Implementation()
{
	//Else if we are unarmed, we play the ROLL UNEQUIP montage only if no other montages are playing
	if (PlayerAnimInstance->IsAnyMontagePlaying() == false)
	{
		if (ActionsMontagesDataTable)
		{
			/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Roll !"));*/

			//Getting montages data from DataTable Row
			FActionsMontages* MontagesStruct = ActionsMontagesDataTable->FindRow<FActionsMontages>(FName(TEXT("UNARMED")), CustomContextString, true);

			//Fetch the corresponding CharacterActions Struct (1st level)
			TMap<ECharacterActions, FAnimMontages> CharacterActions = MontagesStruct->CharacterActions;
			//Get the Montage struct corresponding to the enum we want
			FAnimMontages UnequipAnimMontageStruct = CharacterActions.FindRef(ECharacterActions::RollForward);
			//Get the Montage from the last Montages Struct
			UAnimMontage* FetchedMontage = UnequipAnimMontageStruct.AnimMontage;

			//Plays the montage
			OwnerPlayer->PlayAnimMontage(FetchedMontage, 1.0f);
		}
	}
}

void UActionsComponent::FaceBottom_Button_Sheath_Implementation()
{
	//Only if we are equipped, we play the SHEATH montage only if no other montages are playing
	if (EquipmentManager->CurrentCombatType != ECombatType::Unarmed && PlayerAnimInstance->IsAnyMontagePlaying() == false)
	{
		if (ActionsMontagesDataTable)
		{
			/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Sheath !"));*/

			//Getting montages data from DataTable Row
			FActionsMontages* MontagesStruct = ActionsMontagesDataTable->FindRow<FActionsMontages>(FName(TEXT("SWORD1H")), CustomContextString, true);

			//Fetch the corresponding CharacterActions Struct (1st level)
			TMap<ECharacterActions, FAnimMontages> CharacterActions = MontagesStruct->CharacterActions;
			//Get the Montage struct corresponding to the enum we want
			FAnimMontages UnequipAnimMontageStruct = CharacterActions.FindRef(ECharacterActions::UnequipWeapon);
			//Get the Montage from the last Montages Struct
			UAnimMontage* FetchedMontage = UnequipAnimMontageStruct.AnimMontage;

			//Plays the montage
			OwnerPlayer->PlayAnimMontage(FetchedMontage, 1.0f);			

		}

	}
	
}

void UActionsComponent::FaceBottom_Button_Jump_Implementation(float xAxisValue, float yAxisValue)
{
		//Lateral Jump
		if ((xAxisValue > -0.1f || xAxisValue < 0.1f) && yAxisValue != 0)
		{
			//Jump Right
			if (yAxisValue > 0.0f)
			{
				/*GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("JumpRight !"));*/
				ExecJump(EJumpTypes::ZViewRightJump);

			}
			//Jump Left
			else if (yAxisValue < 0.0f)
			{
				/*GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("JumpLeft !"));*/
				ExecJump(EJumpTypes::ZViewLeftJump);

			}
		}
		//Back-flip
		else if (xAxisValue < -0.0f && (yAxisValue > -0.25f || yAxisValue < 0.25f))
		{
			/*GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Backflip !"));*/
			ExecJump(EJumpTypes::ZViewBackFlip);
		}
		//Jump Attack
		else if (xAxisValue >= 0.0f && (yAxisValue > -0.25f || yAxisValue < 0.25f))
		{
			/*GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("JUMP ATTACK !"));*/
			ExecJump(EJumpTypes::ZViewJumpAttack);
		}

}

void UActionsComponent::ExecJump(EJumpTypes ChosenJump)
{
	//If Player is not falling, we authorize Jump ability
	if (!OwnerPlayer->GetCharacterMovement()->IsFalling() && OwnerPlayer->GetCustomAnimInstance()->IsAnyMontagePlaying() == false)
	{
		//Sets the jump type so the ANIM BP has the info
		SetJumpType(ChosenJump);
		//Attack is disable directly with ANS inside each animation

		//Declaring temporary montage struct
		FActionsMontages* MontagesStruct;

		ASword1H* SwordBase = Cast<ASword1H>(OwnerPlayer->GetEquipmentManager()->LH_Equipped_Gear);
		if (SwordBase || SwordBase == NULL)
		{
			//Getting montages data from DataTable Row
			MontagesStruct = ActionsMontagesDataTable->FindRow<FActionsMontages>(FName(TEXT("SWORD1H")), CustomContextString, true);
		}
		else
		{
			//Getting montages data from DataTable Row
			MontagesStruct = ActionsMontagesDataTable->FindRow<FActionsMontages>(FName(TEXT("UNARMED")), CustomContextString, true);
		}

		switch (ChosenJump)
		{
			case EJumpTypes::ZViewLeftJump:
			{
				if (ActionsMontagesDataTable)
				{

					//Fetch the corresponding CharacterActions Struct (1st level)
					TMap<EJumpTypes, FAnimMontages> JumpTypes = MontagesStruct->Jumps;
					//Get the Montage struct corresponding to the enum we want
					FAnimMontages EquipAnimMontageStruct = JumpTypes.FindRef(ChosenJump);
					//Get the Montage from the last Montages Struct
					UAnimMontage* FetchedMontage = EquipAnimMontageStruct.AnimMontage;

					//Plays the montage
					JumpForce = (OwnerPlayer->GetActorRightVector() * -(JumpDistance)+(OwnerPlayer->GetActorUpVector() * JumpHeight));
					OwnerPlayer->LaunchCharacter(JumpForce, true, true);
					OwnerPlayer->PlayAnimMontage(FetchedMontage, 1.0f);
				}
				break;
			}

			case EJumpTypes::ZViewRightJump:
			{
				if (ActionsMontagesDataTable)
				{

					//Fetch the corresponding CharacterActions Struct (1st level)
					TMap<EJumpTypes, FAnimMontages> JumpTypes = MontagesStruct->Jumps;
					//Get the Montage struct corresponding to the enum we want
					FAnimMontages EquipAnimMontageStruct = JumpTypes.FindRef(ChosenJump);
					//Get the Montage from the last Montages Struct
					UAnimMontage* FetchedMontage = EquipAnimMontageStruct.AnimMontage;

					//Plays the montage
					JumpForce = (OwnerPlayer->GetActorRightVector() * JumpDistance + (OwnerPlayer->GetActorUpVector() * JumpHeight));
					OwnerPlayer->LaunchCharacter(JumpForce, true, true);
					OwnerPlayer->PlayAnimMontage(FetchedMontage, 1.0f);
				}
				break;
			}

			case EJumpTypes::ZViewBackFlip:
			{
				if (ActionsMontagesDataTable)
				{

					//Fetch the corresponding CharacterActions Struct (1st level)
					TMap<EJumpTypes, FAnimMontages> JumpTypes = MontagesStruct->Jumps;
					//Get the Montage struct corresponding to the enum we want
					FAnimMontages EquipAnimMontageStruct = JumpTypes.FindRef(ChosenJump);
					//Get the Montage from the last Montages Struct
					UAnimMontage* FetchedMontage = EquipAnimMontageStruct.AnimMontage;

					//Plays the montage
					JumpForce = (OwnerPlayer->GetActorForwardVector() * -(JumpDistance / 2.25f) + (OwnerPlayer->GetActorUpVector() * JumpHeight * 1.45f));
					OwnerPlayer->LaunchCharacter(JumpForce, true, true);
					OwnerPlayer->PlayAnimMontage(FetchedMontage, 1.0f);
				}
				break;
			}

			case EJumpTypes::ZViewJumpAttack:
			{
				if (ActionsMontagesDataTable)
				{

					//Fetch the corresponding CharacterActions Struct (1st level)
					TMap<EJumpTypes, FAnimMontages> JumpTypes = MontagesStruct->Jumps;
					//Get the Montage struct corresponding to the enum we want
					FAnimMontages EquipAnimMontageStruct = JumpTypes.FindRef(ChosenJump);
					//Get the Montage from the last Montages Struct
					UAnimMontage* FetchedMontage = EquipAnimMontageStruct.AnimMontage;

					//Plays the montage
					JumpForce = (OwnerPlayer->GetActorForwardVector() * (JumpDistance / 2.25f) + (OwnerPlayer->GetActorUpVector() * JumpHeight * 1.45f));
					OwnerPlayer->LaunchCharacter(JumpForce, true, true);
					OwnerPlayer->PlayAnimMontage(FetchedMontage, 1.0f);
				}
				break;
			}

			case EJumpTypes::StandardJump:
			{
				if (ActionsMontagesDataTable)
				{

					//Fetch the corresponding CharacterActions Struct (1st level)
					TMap<EJumpTypes, FAnimMontages> JumpTypes = MontagesStruct->Jumps;
					//Get the Montage struct corresponding to the enum we want
					FAnimMontages EquipAnimMontageStruct = JumpTypes.FindRef(ChosenJump);
					//Get the Montage from the last Montages Struct
					UAnimMontage* FetchedMontage = EquipAnimMontageStruct.AnimMontage;

					//Plays the montage
					JumpForce = (OwnerPlayer->GetActorForwardVector() * (JumpDistance / 2.0f) + (OwnerPlayer->GetActorUpVector() * JumpHeight * 1.6f));
					OwnerPlayer->LaunchCharacter(JumpForce, true, true);
					OwnerPlayer->PlayAnimMontage(FetchedMontage, 1.0f);
				}
				break;
			}
		}
	}
}

void UActionsComponent::RightTrigger_Attack_Implementation()
{
	//SHIELD ATTACK only if not in Z View
	if (EquipmentManager->CurrentCombatType == ECombatType::Shield && CurrentViewType != EViewType::ZView)
	{

		/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("ShieldAttack !"));*/

		if (ActionsMontagesDataTable)
		{
			//UNLOCKING ATTACK ABILITIES
			if (CanAttack == true)
			{
				//Deactivating Attack during the montage (unless authorized later on in cases of combo)
				CanAttack = false;

				//Getting montages data from DataTable Row
				FActionsMontages* MontagesStruct = ActionsMontagesDataTable->FindRow<FActionsMontages>(FName(TEXT("SWORD1H")), CustomContextString, true);

				//Fetch the corresponding CharacterActions Struct (1st level)
				TMap<EAttackTypes, FComboAnimMontages> Attacks = MontagesStruct->Attacks;
				//Get the Montage struct corresponding to the enum we want
				FComboAnimMontages AttacksMontageStruct = Attacks.FindRef(EAttackTypes::ShieldAttack);
				//Get the Montage from the last Montages Struct
				TArray<FAnimMontages> MontagesArray = AttacksMontageStruct.ComboAnimMontage;
				//Size of the montages Array
				AttackIndexMax = (MontagesArray.Num() - 1);
				//Get the Montage Struct based on current Index
				FAnimMontages FetchedMontageStruct = MontagesArray[AttackIndexCurrent];
				UAnimMontage* FetchedMontage = FetchedMontageStruct.AnimMontage;

				//Plays the montage
				OwnerPlayer->PlayAnimMontage(FetchedMontage, 1.0f);

				//Updates the Attack Index Current
				if (AttackIndexCurrent < AttackIndexMax)
				{
					//Incrementing Montage Sequence
					AttackIndexCurrent++;
				}
				else
				{
					//Reseting Montage Sequence
					AttackIndexCurrent = 0;

				}
			}
		}
	}

}

void UActionsComponent::LeftTrigger_ZView_Implementation()
{

	//ZView Camera Timeline Management
	if (ZViewCurveFloat)
	{
		//Get The latest Camera Initial Rotation
		Controller_Initial_Rotation = OwnerPlayer->GetCameraOrientRef()->GetComponentRotation();

		//Get The latest ZView Camera Initial Rotation
		zViewCamera_Initial_Rotation = OwnerPlayer->GetZViewCameraOrientRef()->GetComponentRotation();

		//Start the time lines
		ZViewCameraFTimeline.PlayFromStart();
		ZViewCameraInBackFTimeline.PlayFromStart();
		

		//Set the View so ANIM BP can update accordingly
		SetViewType(EViewType::ZView);

		//Altering the Movement Component to match our needs
		OwnerPlayer->GetMovementComp()->bOrientRotationToMovement = false;

		//Play the corresponding sound from Sound Manager
		OwnerPlayer->GetSoundManager()->PlaySoundBase(OwnerPlayer->GetSoundManager()->ZViewSoundToPlay);

		/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Z View !"));*/

	}

}

void UActionsComponent::LeftTrigger_ClassicView_Implementation()
{
	
	////ZView Camera Timeline Management
	if (ZViewCurveFloat)
	{
		//Get The latest Camera Initial Rotation
		Controller_Initial_Rotation = OwnerPlayer->GetCameraOrientRef()->GetComponentRotation();

		//Get The latest ZView Camera Initial Rotation
		zViewCamera_Initial_Rotation = OwnerPlayer->GetZViewCameraOrientRef()->GetComponentRotation();

		ZViewCameraFTimeline.ReverseFromEnd();
		ZViewRotateYCameraFTimeline.PlayFromStart();

		//Set the View so ANIM BP can update accordingly
		SetViewType(EViewType::Classic);

		//Altering the Movement Component to match our needs
		OwnerPlayer->GetMovementComp()->bOrientRotationToMovement = true;

		/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Classic View !"));*/
	}

}

UDataTable* UActionsComponent::GetActionsMontagesDataTable()
{
		return ActionsMontagesDataTable;
}

void UActionsComponent::SetCanAttack(bool AttackSwitch)
{
	CanAttack = AttackSwitch;
}

void UActionsComponent::ResetCanAttack()
{
	AttackIndexCurrent = 0;
	CanAttack = true;	
	/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("RestCanAttack"));*/
}

void UActionsComponent::SetViewType(EViewType NewViewType)
{
	if (NewViewType != CurrentViewType)
	{
		PrevViewType = CurrentViewType;
		CurrentViewType = NewViewType;

		//Broadcast the new View type so AnimBP updates itself with corresponding set of anims
		OnViewTypeChanged.Broadcast(CurrentViewType);

	}
}

void UActionsComponent::ZViewSpringArmUpdate(float Value)
{
	//Declaring the updated floats that we will use to upsert the various information
	float NewTAL = FMath::Lerp(SpringArm_TargetArmLength, SpringArm_TargetArmLength_End, Value);
	float NewTALZOffset = FMath::Lerp(SpringArm_ZOffset, SpringArm_ZOffset_End, Value);
	OwnerPlayer->GetSpringArmComponent()->TargetArmLength = NewTAL;
	OwnerPlayer->GetSpringArmComponent()->SocketOffset.Z = NewTALZOffset;
	
}

void UActionsComponent::ZViewCameraInBack(float Value)
{
	//Watch Out, control rotation comes in addition to current camera rotation !!
	FRotator NewControlRotation = FMath::Lerp(OwnerPlayer->Controller->GetControlRotation(), zViewCamera_Initial_Rotation, Value);
	OwnerPlayer->Controller->SetControlRotation(NewControlRotation);
}

void UActionsComponent::ZViewRotateYCamera(float Value)
{
	//Watch Out, control rotation comes in addition to current camera rotation !!
	FRotator NewControlRotation = FMath::Lerp(OwnerPlayer->Controller->GetControlRotation(), Controller_Initial_Rotation, Value);
	OwnerPlayer->Controller->SetControlRotation(NewControlRotation);
}

void UActionsComponent::SetJumpType(EJumpTypes NewJumpType)
{
	CurrentJumpType = NewJumpType;

	//Broadcast the new View type so AnimBP updates itself with corresponding set of anims
	OnJumpTypeUsed.Broadcast(CurrentJumpType);

}

void UActionsComponent::SetCanJump(bool JumpSwitch)
{
	CanJump = JumpSwitch;
}