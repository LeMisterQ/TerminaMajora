// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "Animation/AnimInstance.h"
#include "Components/TimelineComponent.h"
#include "ActionsComponent.generated.h"

//Forward Declaration
class ASword1H;

//Context Const String
static const FString CustomContextString(TEXT("Static Context String"));

/// ENUMS ///
UENUM(BlueprintType)
enum class ECharacterActions : uint8 {
	Default				UMETA(DisplayName = "Default"),
	Idle				UMETA(DisplayName = "Idle"),
	RollForward			UMETA(DisplayName = "Roll Forward"),
	EquipWeaponNormal	UMETA(DisplayName = "Equip Weapon Normal Attack"),
	EquipWeaponForward	UMETA(DisplayName = "Equip Weapon Forward Attack"),
	UnequipWeapon		UMETA(DisplayName = "Unequip Weapon"),
	ZViewShielding		UMETA(DisplayName = "ZView Shielding"),
};

UENUM(BlueprintType)
enum class EAttackTypes : uint8 {
	Default					UMETA(DisplayName = "Default"),
	NormalAttack			UMETA(DisplayName = "Normal Attack"),
	ForwardAttack			UMETA(DisplayName = "Forward Attack"),
	ShieldAttack			UMETA(DisplayName = "Shield Attack"),
	ChargedAttack			UMETA(DisplayName = "Charged Attack"),
	ZViewNormalAttack		UMETA(DisplayName = "Z Normal Attack"),
	ZViewForwardAttack		UMETA(DisplayName = "Z Forward Attack"),
	ZViewSideAttack			UMETA(DisplayName = "Z Side Attack"),
};

UENUM(BlueprintType)
enum class EJumpTypes : uint8 {
	Default					UMETA(DisplayName = "Default"),
	ZViewJumpAttack			UMETA(DisplayName = "ZViewJumpAttack"),
	ZViewRightJump			UMETA(DisplayName = "ZViewRightJump"),
	ZViewLeftJump			UMETA(DisplayName = "ZViewLeftJump"),
	ZViewBackFlip			UMETA(DisplayName = "ZViewBackFlip"),
};

UENUM(BlueprintType)
enum class EViewType : uint8 {
	Classic			UMETA(DisplayName = "Classic"),
	ZView			UMETA(DisplayName = "ZView"),
};

/// STRUCTS AND DATATABLES ///
USTRUCT(BlueprintType)
struct FAnimMontages
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PlayRate = 1.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float InTimeToStartMontageAt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ResetTimeMultiplier = 1.0;

};

USTRUCT(BlueprintType)
struct FComboAnimMontages
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FAnimMontages> ComboAnimMontage;

};

USTRUCT(BlueprintType)
struct FActionsMontages : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<ECharacterActions, FAnimMontages> CharacterActions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<EAttackTypes, FComboAnimMontages> Attacks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<EJumpTypes, FAnimMontages> Jumps;

};

//EVENT DISPATCHERS
//OnViewTypeChanged Event Dispatcher
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnViewTypeChanged, EViewType, ViewType);

//OnJumpTypeUsed Event Dispatcher
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJumpTypeUsed, EJumpTypes, JumpType);

//Decoration, there is going to be a pointer on this
class APlayerBase;
class UCurveFloat;
class UAnimInstanceBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TERMINAMAJORA_API UActionsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActionsComponent();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Reference")
		class UEquipmentComponent* EquipmentManager ;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Reference")
		//Used to store the player AnimInstance
		class UAnimInstance* PlayerAnimInstance;

	/// ACTIONS LISTING
	void FaceRight_Button_NormalAttack();
	virtual void FaceRight_Button_NormalAttack_Implementation();

	void FaceRight_Button_ZView_NormalAttack();
	virtual void FaceRight_Button_ZView_NormalAttack_Implementation();

	void FaceRight_Button_ForwardAttack();
	virtual void FaceRight_Button_ForwardAttack_Implementation();

	void FaceRight_Button_ZView_ForwardAttack();
	virtual void FaceRight_Button_ZView_ForwardAttack_Implementation();

	void FaceRight_Button_ZView_SideAttack();
	virtual void FaceRight_Button_ZView_SideAttack_Implementation();

	void FaceRight_Button_ZView_BackwardAttack();
	virtual void FaceRight_Button_ZView_BackwardAttack_Implementation();

	void FaceRight_Button_ChargedAttack();
	virtual void FaceRight_Button_ChargedAttack_Implementation();

	void FaceRight_Button_ChargedAttack_Released();
	virtual void FaceRight_Button_ChargedAttack_Released_Implementation();

	void FaceBottom_Button_Roll();
	virtual void FaceBottom_Button_Roll_Implementation();

	void FaceBottom_Button_Sheath();
	virtual void FaceBottom_Button_Sheath_Implementation();

	void FaceBottom_Button_Jump(float xAxisValue, float yAxisValue);
	virtual void FaceBottom_Button_Jump_Implementation(float xAxisValue, float yAxisValue);

	//Used to store and alter Previous/Current/New View Type
	void ExecJump(EJumpTypes ChosenJump);

	void RightTrigger_Attack();
	virtual void RightTrigger_Attack_Implementation();

	void LeftTrigger_ZView();
	virtual void LeftTrigger_ZView_Implementation();

	void LeftTrigger_ClassicView();
	virtual void LeftTrigger_ClassicView_Implementation();

	//Function Getting the ActionsMontages DataTable so we can use it
	UFUNCTION(BlueprintPure)
		UDataTable* GetActionsMontagesDataTable();

	UFUNCTION()
		void SetCanAttack(bool AttackSwitch);

	UFUNCTION()
		void ResetCanAttack();

	UPROPERTY(BlueprintReadOnly, Category = "Montages")
		ECharacterActions CharacterActionsEnum;

	UPROPERTY(BlueprintReadOnly, Category = "Montages")
		EAttackTypes AttackTypesEnum;

	UPROPERTY(BlueprintReadOnly, Category = "Montages")
		FAnimMontages AnimMontages;

	UPROPERTY(BlueprintReadOnly, Category = "Montages")
		FComboAnimMontages ComboAnimMontages;

	UPROPERTY(BlueprintReadOnly, Category = "Montages")
		FActionsMontages ActionsMontages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montages")
		class UDataTable* ActionsMontagesDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Montages")
		bool CanAttack = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Montages")
		float AttackIndexCurrent = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Montages")
		float AttackIndexMax = 0;

	UPROPERTY(BlueprintReadOnly, Category = "HUD")
		EViewType ViewTypeEnum;

	UPROPERTY(BlueprintAssignable)
		FOnViewTypeChanged OnViewTypeChanged;

	UPROPERTY(BlueprintReadOnly, Category = "View")
		EViewType CurrentViewType = EViewType::Classic;

	UPROPERTY(BlueprintReadOnly, Category = "View")
		EViewType PrevViewType;

	//Used to store and alter Previous/Current/New View Type
		void SetViewType(EViewType NewViewType);

	UFUNCTION()
		void ZViewSpringArmUpdate(float Value);

	UFUNCTION()
		void ZViewCameraInBack(float Value);

	UFUNCTION()
		void ZViewRotateYCamera(float Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
		bool CanJump = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jump")
		float JumpDistance = 800.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jump")
		float JumpHeight = 250.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Jump")
		EJumpTypes JumpTypesEnum;

	UPROPERTY(BlueprintAssignable)
		FOnJumpTypeUsed OnJumpTypeUsed;

	UPROPERTY(BlueprintReadOnly, Category = "Jump")
		EJumpTypes CurrentJumpType;

	//Used to store and alter Previous/Current/New Jump Type
	UFUNCTION()
	void SetJumpType(EJumpTypes NewJumpType);

	//Used to store and alter Previous/Current/New View Type
	UFUNCTION()
	void SetCanJump(bool JumpSwitch);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//ZViewManagement
	FTimeline ZViewCameraFTimeline;

	//ZViewManagement
	FTimeline ZViewCameraInBackFTimeline;

	//ZViewManagement
	FTimeline ZViewRotateYCameraFTimeline;

	UPROPERTY(EditAnywhere, Category = "ZView")
	UCurveFloat* ZViewCurveFloat;

	UPROPERTY(VisibleInstanceOnly, Category = "ZView")
	FRotator Controller_Initial_Rotation = FRotator::ZeroRotator;

	UPROPERTY(VisibleInstanceOnly, Category = "ZView")
	FRotator zViewCamera_Initial_Rotation = FRotator::ZeroRotator;;

	UPROPERTY(VisibleInstanceOnly, Category = "ZView")
	float SpringArm_TargetArmLength;

	UPROPERTY(VisibleInstanceOnly, Category = "ZView")
	float SpringArm_ZOffset;

	UPROPERTY(EditAnywhere, Category = "ZView")
	float SpringArm_TargetArmLength_End = 400.0f;

	UPROPERTY(EditAnywhere, Category = "ZView")
	float SpringArm_ZOffset_End = 170.0f;

	FVector JumpForce;
		

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	//Sets the Owner to the Player 
	APlayerBase* OwnerPlayer;
		
};
