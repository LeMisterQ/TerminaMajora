// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Components/WidgetComponent.h"
#include "UI/UserWidgetBase.h"
#include "HUDBase.generated.h"


UCLASS()
class TERMINAMAJORA_API AHUDBase : public AHUD
{
	GENERATED_BODY()
	
public:
	AHUDBase();

	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "HUD")
		void GoIntoZView();

	UFUNCTION(BlueprintCallable, Category = "HUD")
		void GoIntoClassicView();

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UUserWidget> ZViewWidget;

private:
	UUserWidgetBase* CustomWidget;
};
