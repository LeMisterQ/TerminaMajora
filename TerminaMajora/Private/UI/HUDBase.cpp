// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUDBase.h"

AHUDBase::AHUDBase()
{

}

void AHUDBase::DrawHUD()
{
	Super::DrawHUD();
}

void AHUDBase::BeginPlay()
{
	Super::BeginPlay();

}

void AHUDBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AHUDBase::GoIntoZView()
{

	if (ZViewWidget)
	{

		//Two params : owner of the spawned and the class corresponding
		CustomWidget = CreateWidget<UUserWidgetBase>(GetWorld(), ZViewWidget);

		if (CustomWidget)
		{
			//Render to widget on screen with ZIndex = 0
			CustomWidget->AddToViewport(0);
			CustomWidget->OnDisplayed();
			CustomWidget->isDisplayed = true;
			
		}
	}

}

void AHUDBase::GoIntoClassicView()
{
	if (CustomWidget)
	{
		//Render to widget on screen with ZIndex = 0
		CustomWidget->OnRemoved();
		CustomWidget->isDisplayed = false;
	}
}

