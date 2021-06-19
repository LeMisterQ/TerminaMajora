// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "Animation/AnimInstance.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/EquipmentComponent.h"
#include "Components/ActionsComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/SEComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerBase.generated.h"

//Decoration, there is going to be a pointer on this
class APlayerControllerBase;
class UCurveFloat;

UCLASS()
class TERMINAMAJORA_API APlayerBase : public ACharacterBase
{
	GENERATED_BODY()

	//Scenes handling GEARS meshes (sword, shield, sheath, equipped and unequipped)
	//Expose a component to Blueprint (always this setup for that purpose)
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		USceneComponent* SCN_LH;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		USceneComponent* SCN_RH;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		USceneComponent* SCN_ShieldBack;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		USceneComponent* SCN_SwordBack;


public:
	//Sets default values for this character's properties
	APlayerBase();

	//Declare a variable that corresponds to the MESH COMP for practical reasons
	USkeletalMeshComponent* const SKMesh = GetMesh();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
		class UEquipmentComponent* EquipmentManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
		class UActionsComponent* ActionsManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
		class UInventoryComponent* InventoryManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
		class USEComponent* SoundManager;

	//Function returning all the useful components
	UFUNCTION(BlueprintCallable, BlueprintPure)
	APlayerControllerBase* GetPlayerController();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ACharacterBase* GetParentClass();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UCharacterMovementComponent* GetMovementComp();

	UFUNCTION(BlueprintPure)
	UEquipmentComponent* GetEquipmentManager();

	UFUNCTION(BlueprintPure)
	UActionsComponent* GetActionsManager();

	UFUNCTION(BlueprintPure)
	UInventoryComponent* GetInventoryManager();

	UFUNCTION(BlueprintPure)
	USEComponent* GetSoundManager();

	UFUNCTION(BlueprintPure)
	UAnimInstance* GetPlayerAnimInstance();

	UFUNCTION(BlueprintPure)
	UAnimInstanceBase* GetCustomAnimInstance();

	UFUNCTION(BlueprintPure)
	USceneComponent* Get_LH_SCN();

	UFUNCTION(BlueprintPure)
	USceneComponent* Get_RH_SCN();

	UFUNCTION(BlueprintPure)
	USceneComponent* Get_ShieldBack_SCN();

	UFUNCTION(BlueprintPure)
	USceneComponent* Get_SwordBack_SCN();

	//Parent function override
	USpringArmComponent* GetSpringArmComponent() override;
	UCameraComponent* GetCameraComponent() override;

	//Used to store the player AnimInstance
	class UAnimInstance* PlayerAnimInstance;

	//Used to store Player Controller Reference
	APlayerControllerBase* OwnerController;

	//Used to store the correspondence between SKT Enum and their FName
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		TMap<E_SKTName, FName> SKT_Names_Map;

	//Used to know if we are in a dungeon (useful for a bunch of stuff)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "View")
		bool IsInDungeon = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};