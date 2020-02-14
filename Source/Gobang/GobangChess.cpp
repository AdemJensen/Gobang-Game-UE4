// Fill out your copyright notice in the Description page of Project Settings.

#include "GobangChess.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine.h"
#include "Materials/MaterialInstance.h"

// Sets default values
AGobangChess::AGobangChess()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	Mesh_Black = CreateDefaultSubobject<UStaticMesh>("Mesh_Black");
	auto Mesh_Black_Asset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/GobangObjects/blackChess.blackChess'"));
	if (Mesh_Black_Asset.Object != nullptr)	// if asset not found, then it's nullptr
	{
		Mesh_Black = Mesh_Black_Asset.Object;
		//Mesh->SetStaticMesh(Mesh_Black_Asset.Object);
		Mesh->SetSimulatePhysics(false);
		Mesh->SetGenerateOverlapEvents(false);
		Mesh->SetVisibility(false);
		IsColoredNow = false;
		//Mesh->SetWorldScale3D(FVector(0.25, 0.25, 0.25));
	}

	Mesh_White = CreateDefaultSubobject<UStaticMesh>("Mesh_White");
	auto Mesh_White_Asset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/GobangObjects/whiteChess.whiteChess'"));
	if (Mesh_White_Asset.Object != nullptr)	// if asset not found, then it's nullptr
	{
		Mesh_White = Mesh_White_Asset.Object;
	}

}

// Called when the game starts or when spawned
void AGobangChess::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGobangChess::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGobangChess::SetChessColor(bool IsBlack)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("Set Chess Color %s"), IsBlack == 1 ? TEXT("Black") : TEXT("White")));
	if (IsColoredNow) return;
	if (IsBlack)
	{
		Mesh->SetStaticMesh(Mesh_Black);
		IsColoredNow = true;
		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("SUCCESS"));
	}
	else
	{
		Mesh->SetStaticMesh(Mesh_White);
		IsColoredNow = true;
	}
}

void AGobangChess::SetChessVisibility(bool Visibility)
{
	Mesh->SetVisibility(Visibility);
}
