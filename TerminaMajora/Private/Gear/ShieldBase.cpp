// Fill out your copyright notice in the Description page of Project Settings.


#include "Gear/ShieldBase.h"

AShieldBase::AShieldBase()
{

	//Initializing Shield_1H component
	Shield_1H = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Shield_1H"));
	Shield_1H->AttachToComponent(this->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	/*
	static ConstructorHelpers::FObjectFinder<UStaticMesh>ShieldMeshAsset(TEXT("/Game/Characters/C_Young_Link/Models/Gear/Hylian_Shield_MM.Hylian_Shield_MM"));

	if (ShieldMeshAsset.Succeeded()) {
		UStaticMesh* ShieldAsset = ShieldMeshAsset.Object;
		Shield_1H->SetStaticMesh(ShieldAsset);
	}
	*/

}
