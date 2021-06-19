// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Animation/WidgetAnimation.h"
#include "UserWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class TERMINAMAJORA_API UUserWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UUserWidgetBase(const FObjectInitializer& ObjectInitializer);

	//Act as a Begin Play essentially, but for Widgets
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		bool isDisplayed = false;	

	UFUNCTION(BlueprintNativeEvent)
		void OnDisplayed();
	virtual void OnDisplayed_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void OnRemoved();
	virtual void OnRemoved_Implementation();

	UFUNCTION(BlueprintCallable)
		void DestroyWidget();

};
