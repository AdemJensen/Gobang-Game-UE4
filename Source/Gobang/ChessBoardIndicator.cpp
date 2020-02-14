// Fill out your copyright notice in the Description page of Project Settings.

#include "ChessBoardIndicator.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstance.h"

// Sets default values
AChessBoardIndicator::AChessBoardIndicator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	if (MeshAsset.Object != nullptr)	// if asset not found, then it's nullptr
	{
		Mesh->SetStaticMesh(MeshAsset.Object);
		Mesh->SetSimulatePhysics(false);
		Mesh->SetGenerateOverlapEvents(false);
		Mesh->SetWorldScale3D(FVector(0.25, 0.25, 0.25));
	}

	PlayerMaterial = CreateDefaultSubobject<UMaterial>("PlayerMaterial");
	auto PlayerMaterialAsset = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("Material'/Game/SelectorMaterial_Player.SelectorMaterial_Player'"));
	if (MeshAsset.Object != nullptr)	// if asset not found, then it's nullptr
	{
		PlayerMaterial = PlayerMaterialAsset.Object;
	}

	LastMaterial = CreateDefaultSubobject<UMaterial>("LastMaterial");
	auto LastMaterialAsset = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("Material'/Game/SelectorMaterial_Last.SelectorMaterial_Last'"));
	if (MeshAsset.Object != nullptr)	// if asset not found, then it's nullptr
	{
		LastMaterial = LastMaterialAsset.Object;
		Mesh->SetMaterial(0, LastMaterial);
		IsPlayerNow = false;
	}

	SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void AChessBoardIndicator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChessBoardIndicator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChessBoardIndicator::SetIndicatorColor(bool IsPlayer)
{
	if (IsPlayer)
	{
		if (!IsPlayerNow)
		{
			Mesh->SetMaterial(0, PlayerMaterial);
			IsPlayerNow = true;
		}
	}
	else
	{
		if (IsPlayerNow)
		{
			Mesh->SetMaterial(0, LastMaterial);
			IsPlayerNow = false;
		}
	}
}

void AChessBoardIndicator::SetIndicatorVisibility(bool Visibility)
{
	Mesh->SetVisibility(Visibility);
}
