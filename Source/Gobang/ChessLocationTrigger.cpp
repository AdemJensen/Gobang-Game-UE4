// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessLocationTrigger.h"
#include "Engine.h"

// Sets default values
AChessLocationTrigger::AChessLocationTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Existance = CreateDefaultSubobject<UBoxComponent>(TEXT("Exsitance"));
	Existance->SetNotifyRigidBodyCollision(true);
	Existance->SetGenerateOverlapEvents(true);

	// Set going along with preset "UI"
	Existance->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Existance->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	Existance->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	Existance->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Overlap);
	Existance->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
	Existance->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	Existance->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	Existance->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);
	Existance->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Overlap);
	Existance->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Overlap);
	SetRootComponent(Existance);
}

void AChessLocationTrigger::InitUtility(FVector2D size, int32 Coord_X, int32 Coord_Y, AChessBoardManager* WorldOwnerBoard)
{
	Existance->SetBoxExtent(FVector((size.X / 2) > 0 ? (size.X / 2) : (-size.X / 2) , (size.Y / 2) > 0 ? (size.Y / 2) : (-size.Y / 2), 1));
	//Existance->SetHiddenInGame(false);
	Coordinate_X = Coord_X;
	Coordinate_Y = Coord_Y;
	OwnerBoard = WorldOwnerBoard;
	Existance->OnClicked.AddDynamic(this, &AChessLocationTrigger::TriggerClicked);
}

void AChessLocationTrigger::TriggerClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("Board clicked at (%d, %d)"), Coordinate_X, Coordinate_Y));
	if (OwnerBoard->GetCurrentStatus() == 2)
	{
		if (OwnerBoard->PlaceChess(Coordinate_X, Coordinate_Y, OwnerBoard->GetSelfPlayer() == 1))
		{
			if (OwnerBoard->IsOver())
			{
				//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("OK1")));
				OwnerBoard->SetCurrentStatus(4);
			}
			else
			{
				//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("OK2")));
				OwnerBoard->SetCurrentStatus(3);
				//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("OK3")));
				FTimerHandle UniqueHandle;
				FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject(this, &AChessLocationTrigger::TimerAction);
				GetWorldTimerManager().SetTimer(UniqueHandle, RespawnDelegate, OwnerBoard->GetAiDifficulty() > 2 ? 0.1 : 1, false);
				//OwnerBoard->MakeAiAction();
				//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("OK4")));
			}
		}
	}
	OwnerBoard->HideHintIndicator();
}

void AChessLocationTrigger::TimerAction()
{
	OwnerBoard->MakeAiAction();
}

// Called when the game starts or when spawned
void AChessLocationTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChessLocationTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

