// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardManagerBase.h"
#include "../Basics/BoardTrigger.h"
#include "../Basics/Chess.h"

// Sets default values
ABoardManagerBase::ABoardManagerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

ABoardManagerBase::~ABoardManagerBase()
{
	UWorld* TheWorld = GetWorld();
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (Triggers[i][j] != nullptr)
			{
				Triggers[i][j]->Destroy();
			}
		}
	}
}

void ABoardManagerBase::InitGameBoard(ABoardLocator* LU, ABoardLocator* RD, bool bBanModeOn)
{
	UWorld* TheWorld = GetWorld();
	Location_LU = LU->Location->GetComponentLocation();
	Location_RD = RD->Location->GetComponentLocation();
	

	board.clearBoard();
	board.setBanMode(bBanModeOn);
	
	float boardX = Location_RD.X - Location_LU.X;
	float boardY = Location_RD.Y - Location_LU.Y;
	unitX = boardX / (BOARD_SIZE - 1);
	unitY = boardY / (BOARD_SIZE - 1);

	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("Processing grid(%d, %d)"), i, j));
			ABoardTrigger* NewTrigger = TheWorld->SpawnActor<ABoardTrigger>(Location_LU + FVector(unitX * i, unitY * j, 2), FRotator(0, 0, 0));
			//TheWorld->SpawnActor<ATestActor>(Location_LU + FVector(unitX * i, unitY * j, 0), FRotator(0, 0, 0));
			if (NewTrigger != nullptr)
			{
				NewTrigger->InitUtility(FVector2D(unitX - 1, unitY - 1), i, j, this);
				Triggers[i][j] = NewTrigger;
			}
		}
	}
}

int ABoardManagerBase::PlaceChess(int32 X, int32 Y, bool IsBlack)
{
	if (!board.isAvailable(X, Y, (IsBlack ? 1 : 2))) return false;
	board.placeChess((IsBlack ? 1 : 2), X, Y);
	AChess* ChessObj = GetWorld()->SpawnActor<AChess>(Location_LU + FVector(unitX * X, unitY * Y, 0), FRotator(0, 0, 0));
	if (ChessObj != nullptr)
	{
		ChessObj->SetChessColor(IsBlack);
		ChessObj->SetChessVisibility(true);
	}
	Chesses.Add(ChessObj);
	return true;
}

int32 ABoardManagerBase::GetWinner() {
	return board.getWinner(); 
}

bool ABoardManagerBase::RetractChess()
{
	if (Chesses.Num() < 2) return false;
	board.retract();
	board.retract();
	Chesses[Chesses.Num() - 1]->Destroy();
	Chesses[Chesses.Num() - 2]->Destroy();
	Chesses.RemoveAt(Chesses.Num() - 1);
	Chesses.RemoveAt(Chesses.Num() - 1);
	return true;
}

FIntPoint ABoardManagerBase::GetWinPosition()
{
	if (WinDir == -1)
	{
		std::pair<int, std::pair<int, int> > res = board.getWinLocation();
		WinPos = FIntPoint(res.second.first, res.second.second);
		WinDir = res.first;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("X %d"), WinX));
	return WinPos;
}

int32 ABoardManagerBase::GetWinPositionDir()
{
	if (WinDir == -1)
	{
		std::pair<int, std::pair<int, int> > res = board.getWinLocation();
		WinPos = FIntPoint(res.second.first, res.second.second);
		WinDir = res.first;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("DIR %d"), WinDir));
	return WinDir;
}

FVector ABoardManagerBase::GetAbsolutePosition(int32 X, int32 Y, float Z_Offset)
{
	return FVector(Location_LU.X + unitX * X, Location_LU.Y + unitY * Y, Location_LU.Z + Z_Offset);
}

// Called when the game starts or when spawned
void ABoardManagerBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABoardManagerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

