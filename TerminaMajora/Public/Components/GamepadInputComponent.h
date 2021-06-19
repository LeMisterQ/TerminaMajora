// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/EquipmentComponent.h"
#include "GamepadInputComponent.generated.h"

//Decoration, there is going to be a pointer on this
class APlayerControllerBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TERMINAMAJORA_API UGamepadInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGamepadInputComponent();

	//BlueprintNative allows us to use both Code AND Blueprint after code to do stuff
	UFUNCTION(BlueprintNativeEvent, Category = "Reference")
		void FaceRight_Button();
	virtual void FaceRight_Button_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Reference")
		void FaceRight_Button_Held();
	virtual void FaceRight_Button_Held_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Reference")
		void FaceRight_Button_Released();
	virtual void FaceRight_Button_Released_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Reference")
		void FaceBottom_Button();
	virtual void FaceBottom_Button_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Reference")
		void RightTrigger();
	virtual void RightTrigger_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Reference")
	void RightTrigger_Released();
	virtual void RightTrigger_Released_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Reference")
		void LeftTrigger();
	virtual void LeftTrigger_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Reference")
		void LeftTrigger_Released();
	virtual void LeftTrigger_Released_Implementation();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:

	APlayerBase* CharacterRef;
	APlayerControllerBase* OwnerController;
		
};
