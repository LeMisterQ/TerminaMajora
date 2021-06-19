// Fill out your copyright notice in the Description page of Project Settings.


#include "Gear/WeaponBase.h"

AWeaponBase::AWeaponBase()
{
	//Initializing Sheild_1H Equipped component
	SKNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SK Niagara Component"));
	SKNiagaraComponent->SetupAttachment(AWeaponBase::GetSKMesh());
	SKNiagaraComponent->bAutoActivate = false;

	SMNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SM Niagara Component"));
	SMNiagaraComponent->SetupAttachment(AWeaponBase::GetSMMesh());
	SMNiagaraComponent->bAutoActivate = false;
	
}

void AWeaponBase::ActivateTrail()
{
	//Activate the Niagara System (and resets when it activates)
	if (SKNiagaraComponent)
	{
		SKNiagaraComponent->ReinitializeSystem();
		SKNiagaraComponent->Activate();
	}
	
}

void AWeaponBase::DeactivateTrail()
{
	if (SKNiagaraComponent)
	{
		SKNiagaraComponent->Deactivate();
	}
	
}

