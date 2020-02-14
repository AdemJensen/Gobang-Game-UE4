// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessBoardLocator.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AChessBoardLocator::AChessBoardLocator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Location = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Location"));
	auto Mesh_Asset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/GobangObjects/whiteChess.whiteChess'"));
	if (Mesh_Asset.Object != nullptr)	// if asset not found, then it's nullptr
	{
		Location->SetStaticMesh(Mesh_Asset.Object);
		Location->SetSimulatePhysics(false);
		Location->SetVisibility(false);
		//Mesh->SetWorldScale3D(FVector(0.25, 0.25, 0.25));
	}
	SetRootComponent(Location);
}

// Called when the game starts or when spawned
void AChessBoardLocator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChessBoardLocator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

