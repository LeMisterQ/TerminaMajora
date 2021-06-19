// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidgetBase.h"

//Constructor for the Widget class
UUserWidgetBase::UUserWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UUserWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUserWidgetBase::OnDisplayed_Implementation()
{

}

void UUserWidgetBase::OnRemoved_Implementation()
{

}

void UUserWidgetBase::DestroyWidget()
{
	this->RemoveFromViewport();
}

