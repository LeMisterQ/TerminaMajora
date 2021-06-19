// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CharacterBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values // CONSTRUCTOR
ACharacterBase::ACharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Systematically NOT use the Yaw Rotation of Controller
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	//Declare a variable that corresponds to the MovementComponent for practical reasons
	MovementComponent = GetCharacterMovement();

	//Sets Max Walk Speed custom
	MovementComponent->MaxWalkSpeed = DefaultWalkSpeed;

	//Systematically Orient the Rotation of the Character to the Movement
	MovementComponent->bOrientRotationToMovement = true;
	MovementComponent->bUseControllerDesiredRotation = false;

	GetMesh()->SetOnlyOwnerSee(false);
	GetMesh()->SetOwnerNoSee(false);
	GetMesh()->SetReceivesDecals(false);

	//Initializing SrpingArm Component
	//USE CreateDefaultSubobject ONLY in CONSTRUCTOR - it's a template taking in a class
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(GetRootComponent());

	//Systematically use the Pawn Control Rotation for that component
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetUsingAbsoluteRotation(false);

	//Initializing Camera Component
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->SetRelativeRotation(InitialCameraRotation.Quaternion());

	//Initializing Camera ref Arrow Component
	CameraOrientRef = CreateDefaultSubobject<UArrowComponent>(TEXT("Camera Orientation"));
	CameraOrientRef->SetupAttachment(GetRootComponent());
	CameraOrientRef->SetArrowColor(FColor::Yellow);

	//Initializing ZView Camera ref Arrow Component
	ZViewCameraOrientRef = CreateDefaultSubobject<UArrowComponent>(TEXT("Z-View Camera Orientation"));
	ZViewCameraOrientRef->SetupAttachment(GetRootComponent());
	ZViewCameraOrientRef->SetArrowColor(FColor::Orange);

}

UCameraComponent* ACharacterBase::GetCameraComponent()
{
	return CameraComp;
}

UArrowComponent* ACharacterBase::GetCameraOrientRef()
{
	return CameraOrientRef;
}

UArrowComponent* ACharacterBase::GetZViewCameraOrientRef()
{
	return ZViewCameraOrientRef;
}

USpringArmComponent* ACharacterBase::GetSpringArmComponent()
{
	return SpringArmComp;
}

USkeletalMeshComponent* ACharacterBase::GetSkeletalMeshComponent()
{
	return AddSkelComp;
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

