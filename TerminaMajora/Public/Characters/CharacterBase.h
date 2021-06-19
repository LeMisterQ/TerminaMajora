// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Controller.h"
#include "CharacterBase.generated.h"

//Decoration, there is going to be a pointer on this
class UCameraComponent;

UCLASS()
class TERMINAMAJORA_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()
	

public:
	// Sets default values for this character's properties
	ACharacterBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseComponent")
		UCharacterMovementComponent* MovementComponent;

	//Expose a component to Blueprint (always this setup for that purpose)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseComponent")
		//DECLARING SpringArm Component of the Character
		USpringArmComponent* SpringArmComp;

	//Expose a component to Blueprint (always this setup for that purpose)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseComponent")
		//DECLARING Camera Component of the Character
		UCameraComponent* CameraComp;

	//Expose a component to Blueprint (always this setup for that purpose)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseComponent")
		// DECLARING Additional Skeletal Mesh Component of the Character
		USkeletalMeshComponent* AddSkelComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseComponent")
		UArrowComponent* CameraOrientRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseComponent")
		UArrowComponent* ZViewCameraOrientRef;

	//Getter for the Camera
	virtual UCameraComponent* GetCameraComponent();

	//Getter for the Camera
	virtual UArrowComponent* GetCameraOrientRef();

	//Getter for the Camera
	virtual UArrowComponent* GetZViewCameraOrientRef();

	//Getter for the SA
	virtual USpringArmComponent* GetSpringArmComponent();
	
	//Getter for the SkelMesh
	virtual USkeletalMeshComponent* GetSkeletalMeshComponent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Movement variables
	float DefaultWalkSpeed = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
		FRotator InitialCameraRotation = FRotator(-20.0f, 0.0f, 0.0f);


};
