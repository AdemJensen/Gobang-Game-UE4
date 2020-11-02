// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardTrigger.h"
#include "../GobangGameModeBase.h"
#include "BanMode.h"
#include "../Players/GamePlayerType.h"
#include "../Players/LocalGamePlayer.h"
#include "../Players/LocalRemoteGamePlayer.h"
#include "Engine.h"

// Sets default values
ABoardTrigger::ABoardTrigger()
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

void ABoardTrigger::InitUtility(FVector2D size, int32 Coord_X, int32 Coord_Y, ABoardManagerBase* WorldOwnerBoard)
{
	Existance->SetBoxExtent(FVector((size.X / 2) > 0 ? (size.X / 2) : (-size.X / 2) , (size.Y / 2) > 0 ? (size.Y / 2) : (-size.Y / 2), 1.5));
	//Existance->SetHiddenInGame(false);
	Coordinate_X = Coord_X;
	Coordinate_Y = Coord_Y;
	OwnerBoard = WorldOwnerBoard;
	Existance->OnClicked.AddDynamic(this, &ABoardTrigger::TriggerClicked);
	Existance->OnInputTouchBegin.AddDynamic(this, &ABoardTrigger::OnFingerPressedTrigger);
}

void ABoardTrigger::TriggerClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	HandleClick();
}

void ABoardTrigger::OnFingerPressedTrigger(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	AGobangGameModeBase* MyGameMode = Cast<AGobangGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	EChessType CurrentType = MyGameMode->GameManager->PublicManager->GetCurrentPlayer();
	MyGameMode->GameManager->IndicationManager->HidePlayerIndicator();
	if (MyGameMode->GameManager->PublicManager->GetBanMode() == EBanMode::ON_ILLEGAL_BANNED
		&& MyGameMode->GameManager->BoardManager->IsAvailable(Coordinate_X, Coordinate_Y, CurrentType) != 0
		|| MyGameMode->GameManager->BoardManager->IsAvailable(Coordinate_X, Coordinate_Y, CurrentType) == 1)
	{
		MyGameMode->GameManager->IndicationManager->SetPlayerIndicator(Coordinate_X, Coordinate_Y, false);
	}
	
	HandleClick();
}

void ABoardTrigger::HandleClick()
{
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("Board clicked at (%d, %d)"), Coordinate_X, Coordinate_Y));
	AGobangGameModeBase* MyGameMode = Cast<AGobangGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	EChessType CurrentType = MyGameMode->GameManager->PublicManager->GetCurrentPlayer();
	if (MyGameMode->GameManager->GetProgramStage() == EProgramStage::IN_GAME 
		&& (MyGameMode->GameManager->GetGamePlayer(CurrentType)->GetPlayerType() == EGamePlayerType::LOCAL_PLAYER
			|| MyGameMode->GameManager->GetGamePlayer(CurrentType)->GetPlayerType() == EGamePlayerType::LOCAL_REMOTE_PLAYER))
	{
		if (MyGameMode->GameManager->PublicManager->GetBanMode() == EBanMode::ON_ILLEGAL_BANNED 
			&& MyGameMode->GameManager->BoardManager->IsAvailable(Coordinate_X, Coordinate_Y, CurrentType) != 0
			|| MyGameMode->GameManager->BoardManager->IsAvailable(Coordinate_X, Coordinate_Y, CurrentType) == 1)
		{
			MyGameMode->GameManager->IndicationManager->PlayChessSound(false, MyGameMode->GameManager->IndicationManager->GetAbsolutePosition(Coordinate_X, Coordinate_Y));
		}
		else
		{
			if (MyGameMode->GameManager->GetGamePlayer(CurrentType)->GetPlayerType() == EGamePlayerType::LOCAL_PLAYER) 
			{
				Cast<ALocalGamePlayer>(MyGameMode->GameManager->GetGamePlayer(CurrentType))->DoRoundOver(FIntPoint(Coordinate_X, Coordinate_Y));
			}
			else 
			{
				//bool isNull = Cast<ALocalRemoteGamePlayer>(MyGameMode->GameManager->GetGamePlayer(CurrentType)) == nullptr;
				//UE_LOG(LogTemp, Warning, TEXT("BlogTrigger: PLAYER IS %s."), isNull ? "NULL" : "OK");
				Cast<ALocalRemoteGamePlayer>(MyGameMode->GameManager->GetGamePlayer(CurrentType))->DoRoundOver(FIntPoint(Coordinate_X, Coordinate_Y));
			}
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("BlogTrigger: Click OK."));
}

void ABoardTrigger::TimerAction()
{
	//OwnerBoard->MakeAiAction();
}

// Called when the game starts or when spawned
void ABoardTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoardTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

