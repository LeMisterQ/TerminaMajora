// Fill out your copyright notice in the Description page of Project Settings.


#include "Gear/GearBase.h"
#include "Characters/Player/PlayerBase.h"
#include "Components/ArrowComponent.h"

// Sets default values
AGearBase::AGearBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//COMPONENT MANAGEMENT//
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(RootComp);

	MuzzlePoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Muzzle"));
	MuzzlePoint->ArrowSize = 0.25;
	MuzzlePoint->ArrowColor = FColor::Red;
	MuzzlePoint->SetupAttachment(RootComp);

	BaseSM = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseSM"));
	BaseSM->SetupAttachment(RootComp);
	BaseSM->SetCollisionProfileName(TEXT("OverlapAll"));

	BaseSK = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BaseSK"));
	BaseSK->SetupAttachment(RootComp);
	BaseSM->SetCollisionProfileName(TEXT("OverlapAll"));

	SphereTrace = CreateDefaultSubobject<USphereTraceBase>(TEXT("SphereTrace"));
	SphereTrace->SetupAttachment(RootComp);

}

APlayerBase* AGearBase::GetPlayerOwner()
{
	
	APlayerBase* PlayerOwner = Cast<APlayerBase>(this->GetOwner());
	return PlayerOwner;
	
}

// SET STATIC or SKETELA MESHES for the GEAR
void AGearBase::SetSMMesh_Implementation(UStaticMesh* FetchedSM)
{
	BaseSM->SetStaticMesh(FetchedSM);
}

void AGearBase::SetSKMesh_Implementation(USkeletalMesh* FetchedSK)
{
	BaseSK->SetSkeletalMesh(FetchedSK);
}

UStaticMeshComponent* AGearBase::GetSMMesh()
{
	return BaseSM;
}

USkeletalMeshComponent* AGearBase::GetSKMesh()
{
	return BaseSK;
}

USphereTraceBase* AGearBase::GetSphereTrace_SCN()
{
	return SphereTrace;
}

void AGearBase::TriggerAdvancedHit()
{
	SphereTrace->GenerateAdvancedHit(BaseSK);
}

// Called when the game starts or when spawned
void AGearBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGearBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

