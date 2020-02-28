// Fill out your copyright notice in the Description page of Project Settings.

#include "BoardIndicator.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstance.h"

// Sets default values
ABoardIndicator::ABoardIndicator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	if (MeshAsset.Object != nullptr)	// if asset not found, then it's nullptr
	{
		Mesh->SetStaticMesh(MeshAsset.Object);
		Mesh->SetSimulatePhysics(false);
		Mesh->SetGenerateOverlapEvents(false);
		Mesh->SetWorldScale3D(FVector(0.3, 0.3, 0.3));
	}

	PlayerMaterial = CreateDefaultSubobject<UMaterial>("PlayerMaterial");
	auto PlayerMaterialAsset = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("Material'/Game/SelectorMaterial_Player.SelectorMaterial_Player'"));
	if (PlayerMaterialAsset.Object != nullptr)	// if asset not found, then it's nullptr
	{
		PlayerMaterial = PlayerMaterialAsset.Object;
	}

	LastMaterial = CreateDefaultSubobject<UMaterial>("LastMaterial");
	auto LastMaterialAsset = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("Material'/Game/SelectorMaterial_Last.SelectorMaterial_Last'"));
	if (LastMaterialAsset.Object != nullptr)	// if asset not found, then it's nullptr
	{
		LastMaterial = LastMaterialAsset.Object;
		Mesh->SetMaterial(0, LastMaterial);
		CurrentMat = 2;
	}

	HintMaterial = CreateDefaultSubobject<UMaterial>("HintMaterial");
	auto HintMaterialAsset = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("Material'/Game/SelectorMaterial_Hint.SelectorMaterial_Hint'"));
	if (HintMaterialAsset.Object != nullptr)	// if asset not found, then it's nullptr
	{
		HintMaterial = HintMaterialAsset.Object;
	}

	SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void ABoardIndicator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoardIndicator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABoardIndicator::SetIndicatorColor(int Mat)
{
	if (CurrentMat != Mat)
	{
		switch (Mat)
		{
		case 1:
			Mesh->SetMaterial(0, PlayerMaterial);
			break;
		case 2:
			Mesh->SetMaterial(0, LastMaterial);
			break;
		case 3:
			Mesh->SetMaterial(0, HintMaterial);
			break;
		default:
			return;
		}
		CurrentMat = Mat;
	}
}

void ABoardIndicator::SetIndicatorVisibility(bool Visibility)
{
	Mesh->SetVisibility(Visibility);
}
