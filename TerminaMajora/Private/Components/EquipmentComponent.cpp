// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/EquipmentComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/Player/PlayerBase.h"

// Sets default values for this component's properties
UEquipmentComponent::UEquipmentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

//EVENT DISPATCHER TELLING THE ANIMBP WHAT CACHED STANCE TO USE
void UEquipmentComponent::SetCombatType(ECombatType NewCombatType)
{
	if (NewCombatType != CurrentCombatType)
	{
		PrevCombatType = CurrentCombatType;
		CurrentCombatType = NewCombatType;

		//Broadcast the new combat type so AnimBP updates itself with corresponding set of anims
		OnCombatTypeChanged.Broadcast(CurrentCombatType);

	}
}

void UEquipmentComponent::Set_Current_LH_Equipped_Gear(AGearBase* LH_Gear)
{
	LH_Prev_Equipped_Gear = LH_Equipped_Gear;
	LH_Equipped_Gear = LH_Gear;
	LHGeared = true;
}

void UEquipmentComponent::Set_Current_RH_Equipped_Gear(AGearBase* RH_Gear)
{
	RH_Prev_Equipped_Gear = RH_Equipped_Gear;
	RH_Equipped_Gear = RH_Gear;
	RHGeared = true;
}

void UEquipmentComponent::UnSet_Current_LH_Equipped_Gear(AGearBase* LH_Gear)
{
	LH_Prev_Equipped_Gear = LH_Gear;
	LH_Gear->Destroy();
	LHGeared = false;
}

void UEquipmentComponent::UnSet_Current_RH_Equipped_Gear(AGearBase* RH_Gear)
{
	RH_Prev_Equipped_Gear = RH_Gear;
	RH_Gear->Destroy();
	RHGeared = false;
}

void UEquipmentComponent::SetDefaultGear()
{
	if (GearListDataTable)
	{
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		//////////////////////////// EQUIP BACKSHIELD //////////////////////////////
		//Getting gear data from DataTable Row
		FGearTypeStruct* BackShieldStruct = GearListDataTable->FindRow<FGearTypeStruct>(FName(TEXT("SHIELD_HYLIAN_BACK")), CustomContextString, true);

		//Set the class corresponding to the fetched item to equip
		TSubclassOf<AGearBase> NewBackShield = BackShieldStruct->GearClass;

		//Instantiating the Actor to Attach

		AGearBase* EqBackShield = GetWorld()->SpawnActor<AGearBase>(NewBackShield, Params);

		TMap<E_SKTName, FName> BackShieldMap = BackShieldStruct->SKMeshSKTMap;
		FName BackShieldSKT = BackShieldMap.FindRef(E_SKTName::SKT_BackShield);

		//Find the Enum MeshType telling us if SM or SK
		EMeshType FetchedBackShield = BackShieldStruct->MeshType;
		if (FetchedBackShield == EMeshType::StaticMesh)
		{
			UStaticMesh* FetchedSM = BackShieldStruct->SMMesh;
			EqBackShield->SetSMMesh_Implementation(FetchedSM);
			EqBackShield->AttachToComponent(OwnerPlayer->Get_ShieldBack_SCN(), FAttachmentTransformRules::KeepRelativeTransform);

			UE_LOG(LogTemp, Warning, TEXT("BackShield Attached with success !"));

			//Storing this gear in the dedicated variable
			Equipped_Sheathed_Shield = EqBackShield;
		}

		//////////////////////////// EQUIP SHEATH //////////////////////////////
		//Getting gear data from DataTable Row
		FGearTypeStruct* SheathStruct = GearListDataTable->FindRow<FGearTypeStruct>(FName(TEXT("SHEATH_KOKIRI")), CustomContextString, true);

		//Set the class corresponding to the fetched item to equip
		TSubclassOf<AGearBase> NewSheath = SheathStruct->GearClass;

		//Instantiating the Actor to Attach
		AGearBase* EqSheath = GetWorld()->SpawnActor<AGearBase>(NewSheath, Params);

		TMap<E_SKTName, FName> SheathMap = SheathStruct->SKMeshSKTMap;
		FName SheathSKT = SheathMap.FindRef(E_SKTName::SKT_BackSword);

		//Find the Enum MeshType telling us if SM or SK
		EMeshType FetchedSheath = SheathStruct->MeshType;
		if (FetchedSheath == EMeshType::StaticMesh)
		{
			UStaticMesh* FetchedSM = SheathStruct->SMMesh;
			EqSheath->SetSMMesh_Implementation(FetchedSM);
			EqSheath->AttachToComponent(OwnerPlayer->Get_SwordBack_SCN(), FAttachmentTransformRules::KeepRelativeTransform);

			UE_LOG(LogTemp, Warning, TEXT("Sheath Attached with success !"));

			//Storing this gear in the dedicated variable
			Equipped_Sheath = EqSheath;
		}

		//////////////////////////// EQUIP BACKSWORD //////////////////////////////
		//Getting gear data from DataTable Row
		FGearTypeStruct* BackSwordStruct = GearListDataTable->FindRow<FGearTypeStruct>(FName(TEXT("SWORD_KOKIRI_BACK")), CustomContextString, true);

		//Set the class corresponding to the fetched item to equip
		TSubclassOf<AGearBase> NewBackSword = BackSwordStruct->GearClass;

		//Instantiating the Actor to Attach

		AGearBase* EqBackSword = GetWorld()->SpawnActor<AGearBase>(NewBackSword, Params);

		TMap<E_SKTName, FName> BackSwordMap = BackSwordStruct->SKMeshSKTMap;
		FName BackSwordSKT = BackSwordMap.FindRef(E_SKTName::SKT_BackSword);

		//Find the Enum MeshType telling us if SM or SK
		EMeshType FetchedBackSword = BackSwordStruct->MeshType;
		if (FetchedBackSword == EMeshType::StaticMesh)
		{
			UStaticMesh* FetchedSM = BackSwordStruct->SMMesh;
			EqBackSword->SetSMMesh_Implementation(FetchedSM);
			EqBackSword->AttachToComponent(OwnerPlayer->Get_SwordBack_SCN(), FAttachmentTransformRules::KeepRelativeTransform);

			UE_LOG(LogTemp, Warning, TEXT("BackSword Attached with success !"));

			//Storing this gear in the dedicated variable
			Equipped_Sheathed_Sword = EqBackSword;
		}
	}

}

void UEquipmentComponent::Equip_SwordShield1H()
{
	//////////////////////////// EQUIP SWORD AND SHIELD //////////////////////////////

	//If we are unequipped
	if (CurrentCombatType == ECombatType::Unarmed && GearListDataTable->IsValidLowLevel() && (RHGeared == false && LHGeared == false))
	{

		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		//Getting gear data from DataTable Row
		FGearTypeStruct* SwordStruct = GearListDataTable->FindRow<FGearTypeStruct>(FName(TEXT("SWORD_KOKIRI")), CustomContextString, true);
		FGearTypeStruct* ShieldStruct = GearListDataTable->FindRow<FGearTypeStruct>(FName(TEXT("SHIELD_HYLIAN")), CustomContextString, true);

		//Set the class corresponding to the fetched item to equip
		TSubclassOf<AGearBase> NewWeapon = SwordStruct->GearClass;
		TSubclassOf<AGearBase> NewShield = ShieldStruct->GearClass;

		//Instantiating the Actor to Attach
		AGearBase* EqSword = GetWorld()->SpawnActor<AGearBase>(NewWeapon, Params);
		AGearBase* EqShield = GetWorld()->SpawnActor<AGearBase>(NewShield, Params);

		TMap<E_SKTName, FName> SwordMap = SwordStruct->SKMeshSKTMap;
		FName SwordSKT = SwordMap.FindRef(E_SKTName::SKT_Sword1H);
		TMap<E_SKTName, FName> ShieldMap = ShieldStruct->SKMeshSKTMap;
		FName ShieldSKT = ShieldMap.FindRef(E_SKTName::SKT_Shield1H);

		//SWORD
		//Find the Enum MeshType telling us if SM or SK
		EMeshType FetchedSwordMeshType = SwordStruct->MeshType;
		if (FetchedSwordMeshType == EMeshType::StaticMesh)
		{
			UStaticMesh* FetchedSM = SwordStruct->SMMesh;

			EqSword->SetSMMesh_Implementation(FetchedSM);
			EqSword->AttachToComponent(OwnerPlayer->Get_LH_SCN(), FAttachmentTransformRules::KeepRelativeTransform);

			UE_LOG(LogTemp, Warning, TEXT("LH STATIC Weapon attached with success !"));

			//Setting as Equipped
			Set_Current_LH_Equipped_Gear(EqSword);

			//Removing BackSword when equipped
			Equipped_Sheathed_Sword->SetActorHiddenInGame(true);
			
		}
		else if (FetchedSwordMeshType == EMeshType::SkeletalMesh)
		{
			USkeletalMesh* FetchedSK = SwordStruct->SKMesh;

			EqSword->SetSKMesh_Implementation(FetchedSK);
			EqSword->AttachToComponent(OwnerPlayer->Get_LH_SCN(), FAttachmentTransformRules::KeepRelativeTransform);

			UE_LOG(LogTemp, Warning, TEXT("LH SKELETAL Weapon attached with success !"));

			//Setting as Equipped
			Set_Current_LH_Equipped_Gear(EqSword);

			//Removing BackSword when equipped
			Equipped_Sheathed_Sword->SetActorHiddenInGame(true);
		}

		//SHIELD
		EMeshType FetchedShieldMeshType = ShieldStruct->MeshType;
		if (FetchedShieldMeshType == EMeshType::StaticMesh)
		{
			UStaticMesh* FetchedSM = ShieldStruct->SMMesh;

			EqShield->SetSMMesh_Implementation(FetchedSM);
			EqShield->AttachToComponent(OwnerPlayer->Get_RH_SCN(), FAttachmentTransformRules::KeepRelativeTransform);

			UE_LOG(LogTemp, Warning, TEXT("RH STATIC Shield attached with success !"));

			//Setting as Equipped
			Set_Current_RH_Equipped_Gear(EqShield);

			//Removing BackShield when equipped
			Equipped_Sheathed_Shield->SetActorHiddenInGame(true);

		}
		else if (FetchedShieldMeshType == EMeshType::SkeletalMesh)
		{
			USkeletalMesh* FetchedSK = ShieldStruct->SKMesh;

			EqShield->SetSKMesh_Implementation(FetchedSK);
			EqShield->AttachToComponent(OwnerPlayer->Get_RH_SCN(), FAttachmentTransformRules::KeepRelativeTransform);

			UE_LOG(LogTemp, Warning, TEXT("RH SKELETAL Shield attached with success !"));

			//Setting as Equipped
			Set_Current_RH_Equipped_Gear(EqShield);

			//Removing BackShield when equipped
			Equipped_Sheathed_Shield->SetActorHiddenInGame(true);
		}

		//Sets the new Combat Type (Event Dispatcher)
		SetCombatType(ECombatType::Sword1H);

	}
	//Useful for exiting Shield Combat Type
	else
	{

		//If we are only equipped with LH, then unequip all
		if (RHGeared == false || LHGeared == false)
		{
			UnEquip_SwordShield1H();
		}
		else {

			//Sets the new Combat Type (Event Dispatcher)
			SetCombatType(ECombatType::Sword1H);
		}

	}

}

void UEquipmentComponent::UnEquip_SwordShield1H()
{
	//////////////////////////// UNEQUIP AnyWeapon //////////////////////////////

	//If we are equipped
	if (CurrentCombatType != ECombatType::Unarmed && GearListDataTable->IsValidLowLevel())
	{
		
		if (LHGeared == true)
		{
			UnSet_Current_LH_Equipped_Gear(LH_Equipped_Gear);
		}
		if (RHGeared == true)
		{
			UnSet_Current_RH_Equipped_Gear(RH_Equipped_Gear);
		}
		if (Equipped_Sheathed_Sword != NULL && Equipped_Sheathed_Shield != NULL)
		{
			Equipped_Sheathed_Sword->SetActorHiddenInGame(false);
			Equipped_Sheathed_Shield->SetActorHiddenInGame(false);
		}

		//Sets the new Combat Type (Event Dispatcher)
		SetCombatType(ECombatType::Unarmed);
	}

}

void UEquipmentComponent::Equip_Shield1H()
{
	if (GearListDataTable->IsValidLowLevel())
	{

		//If we try to equip shield and we are unarmed
		if (CurrentCombatType == ECombatType::Unarmed && GearListDataTable->IsValidLowLevel() && (RHGeared == false && LHGeared == false))
		{		
			
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			//Getting gear data from DataTable Row
			FGearTypeStruct* SwordStruct = GearListDataTable->FindRow<FGearTypeStruct>(FName(TEXT("SWORD_KOKIRI")), CustomContextString, true);
			FGearTypeStruct* ShieldStruct = GearListDataTable->FindRow<FGearTypeStruct>(FName(TEXT("SHIELD_HYLIAN")), CustomContextString, true);

			//Set the class corresponding to the fetched item to equip
			TSubclassOf<AGearBase> NewWeapon = SwordStruct->GearClass;
			TSubclassOf<AGearBase> NewShield = ShieldStruct->GearClass;

			//Instantiating the Actor to Attach
			AGearBase* EqSword = GetWorld()->SpawnActor<AGearBase>(NewWeapon, Params);
			AGearBase* EqShield = GetWorld()->SpawnActor<AGearBase>(NewShield, Params);

			TMap<E_SKTName, FName> SwordMap = SwordStruct->SKMeshSKTMap;
			FName SwordSKT = SwordMap.FindRef(E_SKTName::SKT_Sword1H);
			TMap<E_SKTName, FName> ShieldMap = ShieldStruct->SKMeshSKTMap;
			FName ShieldSKT = ShieldMap.FindRef(E_SKTName::SKT_Shield1H);

			//SWORD
			//Find the Enum MeshType telling us if SM or SK
			EMeshType FetchedSwordMeshType = SwordStruct->MeshType;
			if (FetchedSwordMeshType == EMeshType::StaticMesh)
			{
				UStaticMesh* FetchedSM = SwordStruct->SMMesh;

				EqSword->SetSMMesh_Implementation(FetchedSM);
				EqSword->AttachToComponent(OwnerPlayer->Get_LH_SCN(), FAttachmentTransformRules::KeepRelativeTransform);

				UE_LOG(LogTemp, Warning, TEXT("LH STATIC Weapon attached with success !"));

				//Setting as Equipped
				Set_Current_LH_Equipped_Gear(EqSword);

				//Removing BackSword when equipped
				Equipped_Sheathed_Sword->SetActorHiddenInGame(true);

			}
			else if (FetchedSwordMeshType == EMeshType::SkeletalMesh)
			{
				USkeletalMesh* FetchedSK = SwordStruct->SKMesh;

				EqSword->SetSKMesh_Implementation(FetchedSK);
				EqSword->AttachToComponent(OwnerPlayer->Get_LH_SCN(), FAttachmentTransformRules::KeepRelativeTransform);

				UE_LOG(LogTemp, Warning, TEXT("LH SKELETAL Weapon attached with success !"));

				//Setting as Equipped
				Set_Current_LH_Equipped_Gear(EqSword);

				//Removing BackSword when equipped
				Equipped_Sheathed_Sword->SetActorHiddenInGame(true);
			}

			//SHIELD
			EMeshType FetchedShieldMeshType = ShieldStruct->MeshType;
			if (FetchedShieldMeshType == EMeshType::StaticMesh)
			{
				UStaticMesh* FetchedSM = ShieldStruct->SMMesh;

				EqShield->SetSMMesh_Implementation(FetchedSM);
				EqShield->AttachToComponent(OwnerPlayer->Get_RH_SCN(), FAttachmentTransformRules::KeepRelativeTransform);

				UE_LOG(LogTemp, Warning, TEXT("RH STATIC Shield attached with success !"));

				//Setting as Equipped
				Set_Current_RH_Equipped_Gear(EqShield);

				//Removing BackShield when equipped
				Equipped_Sheathed_Shield->SetActorHiddenInGame(true);

			}
			else if (FetchedShieldMeshType == EMeshType::SkeletalMesh)
			{
				USkeletalMesh* FetchedSK = ShieldStruct->SKMesh;

				EqShield->SetSKMesh_Implementation(FetchedSK);
				EqShield->AttachToComponent(OwnerPlayer->Get_RH_SCN(), FAttachmentTransformRules::KeepRelativeTransform);

				UE_LOG(LogTemp, Warning, TEXT("RH SKELETAL Shield attached with success !"));

				//Setting as Equipped
				Set_Current_RH_Equipped_Gear(EqShield);

				//Removing BackShield when equipped
				Equipped_Sheathed_Shield->SetActorHiddenInGame(true);
			}			


			//Sets the new Combat Type (Event Dispatcher)
			SetCombatType(ECombatType::Shield);			

			//We set CanAttack = True in the Action Manager (since it's not determined by any AnimMontage unlike all Equip Actions)
			OwnerPlayer->GetActionsManager()->ResetCanAttack();

		}
		else
		{
			//Sets the new Combat Type (Event Dispatcher)
			SetCombatType(ECombatType::Shield);

			//We set CanAttack = True in the Action Manager (since it's not determined by any AnimMontage)
			OwnerPlayer->GetActionsManager()->ResetCanAttack();
		}

		//Play ShieldOut Sound (set up directly within the BluePrint to avoid hard-coding file path)
		UGameplayStatics::PlaySoundAtLocation(this, OwnerPlayer->GetSoundManager()->ShieldOutSound, OwnerPlayer->GetActorLocation());
			
	}
}

void UEquipmentComponent::UnEquip_Shield1H()
{
	//If We were unarmed before equipping shield, we go back to being unarmed (and we have to delete corresponding meshes)
	if (PrevCombatType == ECombatType::Unarmed && OwnerPlayer->GetPlayerAnimInstance()->IsAnyMontagePlaying() == false && GearListDataTable->IsValidLowLevel())
	{

		//Since we Equip Shield1H, we assume we are coming from holding a 1H weapon
		Equip_SwordShield1H();

	}
	else if (PrevCombatType != ECombatType::Unarmed && OwnerPlayer->GetPlayerAnimInstance()->IsAnyMontagePlaying() == false && GearListDataTable->IsValidLowLevel())
	{
		SetCombatType(PrevCombatType);
	}

	//Play ShieldIn Sound (set up directly within the BluePrint to avoid hard-coding file path)
	UGameplayStatics::PlaySoundAtLocation(this, OwnerPlayer->GetSoundManager()->ShieldInSound, OwnerPlayer->GetActorLocation());
}

void UEquipmentComponent::Equip_Shield_Only()
{
	if (GearListDataTable->IsValidLowLevel())
	{

		//If we try to equip shield and we are unarmed
		if (CurrentCombatType == ECombatType::Unarmed && GearListDataTable->IsValidLowLevel() && (RHGeared == false && LHGeared == false))
		{

			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			//Getting gear data from DataTable Row
			FGearTypeStruct* ShieldStruct = GearListDataTable->FindRow<FGearTypeStruct>(FName(TEXT("SHIELD_HYLIAN")), CustomContextString, true);

			//Set the class corresponding to the fetched item to equip
			TSubclassOf<AGearBase> NewShield = ShieldStruct->GearClass;

			//Instantiating the Actor to Attach
			AGearBase* EqShield = GetWorld()->SpawnActor<AGearBase>(NewShield, Params);

			TMap<E_SKTName, FName> ShieldMap = ShieldStruct->SKMeshSKTMap;
			FName ShieldSKT = ShieldMap.FindRef(E_SKTName::SKT_Shield1H);

			//SHIELD
			EMeshType FetchedShieldMeshType = ShieldStruct->MeshType;
			if (FetchedShieldMeshType == EMeshType::StaticMesh)
			{
				UStaticMesh* FetchedSM = ShieldStruct->SMMesh;

				EqShield->SetSMMesh_Implementation(FetchedSM);
				EqShield->AttachToComponent(OwnerPlayer->Get_RH_SCN(), FAttachmentTransformRules::KeepRelativeTransform);

				UE_LOG(LogTemp, Warning, TEXT("RH STATIC Shield attached with success !"));

				//Setting as Equipped
				Set_Current_RH_Equipped_Gear(EqShield);

				//Removing BackShield when equipped
				Equipped_Sheathed_Shield->SetActorHiddenInGame(true);

			}
			else if (FetchedShieldMeshType == EMeshType::SkeletalMesh)
			{
				USkeletalMesh* FetchedSK = ShieldStruct->SKMesh;

				EqShield->SetSKMesh_Implementation(FetchedSK);
				EqShield->AttachToComponent(OwnerPlayer->Get_RH_SCN(), FAttachmentTransformRules::KeepRelativeTransform);

				UE_LOG(LogTemp, Warning, TEXT("RH SKELETAL Shield attached with success !"));

				//Setting as Equipped
				Set_Current_RH_Equipped_Gear(EqShield);

				//Removing BackShield when equipped
				Equipped_Sheathed_Shield->SetActorHiddenInGame(true);
			}

			//Sets the new Combat Type (Event Dispatcher)
			SetCombatType(ECombatType::Shield);

			//We set CanAttack = True in the Action Manager (since it's not determined by any AnimMontage unlike all Equip Actions)
			OwnerPlayer->GetActionsManager()->ResetCanAttack();

		}
		else
		{
			//Sets the new Combat Type (Event Dispatcher)
			SetCombatType(ECombatType::Shield);

			//We set CanAttack = True in the Action Manager (since it's not determined by any AnimMontage)
			OwnerPlayer->GetActionsManager()->ResetCanAttack();
		}

		//Play ShieldOut Sound (set up directly within the BluePrint to avoid hard-coding file path)
		UGameplayStatics::PlaySoundAtLocation(this, OwnerPlayer->GetSoundManager()->ShieldOutSound, OwnerPlayer->GetActorLocation());

	}
}

void UEquipmentComponent::UnEquip_Shield_Only()
{
	//If we were unarmed before equipping shield ONLY, we go back to being unarmed (and we have to delete corresponding meshes)
	if (PrevCombatType == ECombatType::Unarmed && OwnerPlayer->GetPlayerAnimInstance()->IsAnyMontagePlaying() == false && GearListDataTable->IsValidLowLevel())
	{
		//Since we Equip Shield1H, we assume we are coming from holding a 1H weapon
		UnEquip_SwordShield1H();

	}
	else if (PrevCombatType != ECombatType::Unarmed && OwnerPlayer->GetPlayerAnimInstance()->IsAnyMontagePlaying() == false && GearListDataTable->IsValidLowLevel())
	{
		SetCombatType(PrevCombatType);
	}

	//Play ShieldIn Sound (set up directly within the BluePrint to avoid hard-coding file path)
	UGameplayStatics::PlaySoundAtLocation(this, OwnerPlayer->GetSoundManager()->ShieldInSound, OwnerPlayer->GetActorLocation());
}

// Called when the game starts
void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	//Declaring OwnerPlayer firstly
	OwnerPlayer = Cast<APlayerBase>(this->GetOwner());
	GearListDataTable = OwnerPlayer->GetInventoryManager()->GetItemListDataTable();

	//Sets the default Gear (Kokiri Gear)
	SetDefaultGear();
	
}

// Called every frame
void UEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

