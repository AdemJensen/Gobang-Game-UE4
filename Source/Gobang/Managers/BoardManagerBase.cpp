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

void ABoardManagerBase::RemoveGameBoard()
{
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

int ABoardManagerBase::PlaceChess(int32 X, int32 Y, EChessType ChessType)
{
	WinDir = -1;
	int Avail = IsAvailable(X, Y, ChessType);
	if (Avail == 1) return 1;
	board.placeChess((ChessType == EChessType::BLACK ? 1 : 2), X, Y);
	AChess* ChessObj = GetWorld()->SpawnActor<AChess>(Location_LU + FVector(unitX * X, unitY * Y, 1), FRotator(0, 0, 0));
	if (ChessObj != nullptr)
	{
		ChessObj->SetChessColor(ChessType == EChessType::BLACK);
		ChessObj->SetChessVisibility(true);
	}
	Chesses.Add(ChessObj);
	return Avail;
}

TArray<FIntPoint> ABoardManagerBase::GetReviewData()
{
	std::stack< std::pair<int, int> > ReviewData = board.getReviewData();
	TArray<FIntPoint> Result;
	while (!ReviewData.empty())
	{
		Result.Insert(FIntPoint(ReviewData.top().first, ReviewData.top().second), 0);
		ReviewData.pop();
	}
	return Result;
}

int32 ABoardManagerBase::GetWinner() {
	return board.getWinner(); 
}

std::pair<FIntPoint, FIntPoint> ABoardManagerBase::RetractChess()
{
	WinDir = -1;
	if (Chesses.Num() < 2) return std::make_pair(FIntPoint(-1, -1), FIntPoint(-1, -1));
	FIntPoint Point1, Point2;
	Point1 = FIntPoint(board.getLastChess().first, board.getLastChess().second);
	board.retract();
	Point2 = FIntPoint(board.getLastChess().first, board.getLastChess().second);
	board.retract();
	Chesses[Chesses.Num() - 1]->Destroy();
	Chesses[Chesses.Num() - 2]->Destroy();
	Chesses.RemoveAt(Chesses.Num() - 1);
	Chesses.RemoveAt(Chesses.Num() - 1);
	return std::make_pair(Point1, Point2);
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

FIntPoint ABoardManagerBase::GetLastPosition()
{
	if (board.isEmpty()) return FIntPoint(-1, -1);
	std::pair<int, int> Loc = board.getLastChess();
	return FIntPoint(Loc.first, Loc.second);
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

