// Fill out your copyright notice in the Description page of Project Settings.


#include "Gear/SheathBase.h"

ASheathBase::ASheathBase()
{

	//Initializing Sheath component
	Sheath_Back = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Sheath_Back"));
	Sheath_Back->AttachToComponent(this->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	/*
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Sheath_BackMeshAsset(TEXT("/Game/Characters/C_Young_Link/Models/Gear/Kokiri_Sayakuta_MM.Kokiri_Sayakuta_MM"));

	if (Sheath_BackMeshAsset.Succeeded()) {
		UStaticMesh* Sheath_BackAsset = Sheath_BackMeshAsset.Object;
		Sheath_Back->SetStaticMesh(Sheath_BackAsset);
	}
	*/

}
