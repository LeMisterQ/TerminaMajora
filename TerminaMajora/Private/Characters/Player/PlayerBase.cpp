// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/PlayerBase.h"
#include "Characters/Player/PlayerControllerBase.h"
#include "UObject/ConstructorHelpers.h"


APlayerBase::APlayerBase()
{
	//Initializing EquipmentManager
	EquipmentManager = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipmentManager"));

	//Initializing Actions Component
	ActionsManager = CreateDefaultSubobject<UActionsComponent>(TEXT("ActionsManager"));

	//Initializing Inventory Component
	InventoryManager = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryManager"));

	//Initializing SEComponent
	SoundManager = CreateDefaultSubobject<USEComponent>(TEXT("SoundManager"));

	//Initializing SKT Names
	SKT_Names_Map.Add(E_SKTName::None, "None");
	SKT_Names_Map.Add(E_SKTName::SKT_Sword1H, "SKT_Sword1H");
	SKT_Names_Map.Add(E_SKTName::SKT_Shield1H, "SKT_Shield1H");
	SKT_Names_Map.Add(E_SKTName::SKT_BackSword, "SKT_BackSword");
	SKT_Names_Map.Add(E_SKTName::SKT_BackShield, "SKT_BackShield");

	//Initializing Link SKM as default SKM
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> HeroeMeshAsset(TEXT("/Game/Characters/C_Young_Link/Models/SK_Youg_Link.SK_Youg_Link"));
	USkeletalMesh* HeroeAsset = HeroeMeshAsset.Object;
	SKMesh->SetSkeletalMesh(HeroeAsset);
	
	///// SOCKET MANAGEMENT FROM DATATABLE ///// 
	//Getting Sword data from stored Map
	FName SwordSKT = SKT_Names_Map.FindRef(E_SKTName::SKT_Sword1H);

	//Getting Shield data from stored Map
	FName ShieldSKT = SKT_Names_Map.FindRef(E_SKTName::SKT_Shield1H);

	//Getting BackShield data from stored Map
	FName BackShieldSKT = SKT_Names_Map.FindRef(E_SKTName::SKT_BackShield);

	//Getting BackSword data from stored Map
	FName BackSwordSKT = SKT_Names_Map.FindRef(E_SKTName::SKT_BackSword);

	///// CREATING SCENES WITH CORRECT SOCKETS /////  
	//Initializing Sword_1H Equipped component
	SCN_LH = CreateDefaultSubobject<USceneComponent>(TEXT("SCN_Sword_1H"));

	//USE SetupAttachment ONLY in CONSTRUCTOR, not at RUNTIME
	SCN_LH->AttachToComponent(SKMesh, FAttachmentTransformRules::KeepRelativeTransform, SwordSKT);

	//Initializing Sheild_1H Equipped component
	SCN_RH = CreateDefaultSubobject<USceneComponent>(TEXT("SCN_RH"));
	SCN_RH->AttachToComponent(SKMesh, FAttachmentTransformRules::KeepRelativeTransform, ShieldSKT);

	//Initializing ShieldBack UNequipped component
	SCN_ShieldBack = CreateDefaultSubobject<USceneComponent>(TEXT("SCN_ShieldBack"));
	SCN_ShieldBack->AttachToComponent(SKMesh, FAttachmentTransformRules::KeepRelativeTransform, BackShieldSKT);

	//Initializing SwordBack UNequipped component
	SCN_SwordBack = CreateDefaultSubobject<USceneComponent>(TEXT("SCN_SwordBack"));
	SCN_SwordBack->AttachToComponent(SKMesh, FAttachmentTransformRules::KeepRelativeTransform, BackSwordSKT);	

}

/// USEFUL GETTERS ///

UEquipmentComponent* APlayerBase::GetEquipmentManager()
{
	return EquipmentManager;
}

UActionsComponent* APlayerBase::GetActionsManager()
{
	return ActionsManager;
}

UInventoryComponent* APlayerBase::GetInventoryManager()
{
	return InventoryManager;
}

USEComponent* APlayerBase::GetSoundManager()
{
	return SoundManager;
}

APlayerControllerBase* APlayerBase::GetPlayerController()
{
	OwnerController = Cast<APlayerControllerBase>(Controller);
	return OwnerController;
}

ACharacterBase* APlayerBase::GetParentClass()
{
	ACharacterBase* ParentOwner = Cast<ACharacterBase>(this);
	return ParentOwner;
}

UCharacterMovementComponent* APlayerBase::GetMovementComp()
{

	UCharacterMovementComponent* ParentMovComponent = GetParentClass()->GetCharacterMovement();
	return ParentMovComponent;
}

USceneComponent* APlayerBase::Get_LH_SCN()
{
	return SCN_LH;
}

USceneComponent* APlayerBase::Get_RH_SCN()
{
	return SCN_RH;
}

USceneComponent* APlayerBase::Get_ShieldBack_SCN()
{
	return SCN_ShieldBack;
}

USceneComponent* APlayerBase::Get_SwordBack_SCN()
{
	return SCN_SwordBack;
}

USpringArmComponent* APlayerBase::GetSpringArmComponent()
{
	return Super::GetSpringArmComponent();
}

UCameraComponent* APlayerBase::GetCameraComponent()
{
	return Super::GetCameraComponent();
}

UAnimInstance* APlayerBase::GetPlayerAnimInstance()
{

	PlayerAnimInstance = SKMesh->GetAnimInstance();
	return PlayerAnimInstance;
	
}

UAnimInstanceBase* APlayerBase::GetCustomAnimInstance()
{
	UAnimInstanceBase* CustomAnimInstance = Cast<UAnimInstanceBase>(this->GetPlayerAnimInstance());
	return CustomAnimInstance;
}

// Called when the game starts
void APlayerBase::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}



