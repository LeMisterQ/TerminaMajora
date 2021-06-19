// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Gear/GearBase.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "InventoryComponent.generated.h"

/// ENUMS ///
UENUM(BlueprintType)
enum class EMeshType : uint8 {
	None			UMETA(DisplayName = "None"),
	SkeletalMesh	UMETA(DisplayName = "Skeletal Mesh"),
	StaticMesh		UMETA(DisplayName = "Static Mesh"),
};

UENUM(BlueprintType)
enum class E_SKTName : uint8 {
	None			UMETA(DisplayName = "None"),
	SKT_Sword1H		UMETA(DisplayName = "SKT_Sword1H"),
	SKT_Shield1H	UMETA(DisplayName = "SKT_Shield1H"),
	SKT_BackShield	UMETA(DisplayName = "SKT_BackShield"),
	SKT_BackSword	UMETA(DisplayName = "SKT_BackSword"),
};

/// STRUCTS AND DATATABLES ///
USTRUCT(BlueprintType)
struct FGearTypeStruct : public FTableRowBase
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EMeshType MeshType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AGearBase> GearClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMesh* SKMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMesh* SMMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<E_SKTName, FName> SKMeshSKTMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName Description;

};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TERMINAMAJORA_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	//Function Getting the ItemList so we can use it
	UDataTable* GetItemListDataTable();

	UPROPERTY(BlueprintReadOnly, Category = "Gear")
		EMeshType MeshTypeEnum;

	UPROPERTY(BlueprintReadOnly, Category = "Gear")
		FGearTypeStruct GearTypeStruct;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gear")
		class UDataTable* GearListDataTable;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
