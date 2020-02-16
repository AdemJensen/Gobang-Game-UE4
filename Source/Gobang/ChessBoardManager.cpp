// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessBoardManager.h"
#include "ChessLocationTrigger.h"
#include "GobangChess.h"
#include "ThreadAiWorker.h"

// Sets default values
AChessBoardManager::AChessBoardManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Indicator_Player = CreateDefaultSubobject<AChessBoardIndicator>("Indicator_Player");
	Indicator_Last = CreateDefaultSubobject<AChessBoardIndicator>("Indicator_Last");
	Indicator_Hint = CreateDefaultSubobject<AChessBoardIndicator>("Indicator_Hint");
	PlaceChessAudio = CreateDefaultSubobject<USoundWave>("PlaceChessAudio");

	DisableGameboard();
}

void AChessBoardManager::InitGameBoard(bool isViewOnBlack)
{
	Location_LU = Locator_LU->Location->GetComponentLocation();
	Location_RD = Locator_RD->Location->GetComponentLocation();
	UWorld* TheWorld = GetWorld();
	Indicator_Player = TheWorld->SpawnActor<AChessBoardIndicator>(Location_LU + FVector(0, 0, 2), FRotator(0, 0, 0));
	Indicator_Last = TheWorld->SpawnActor<AChessBoardIndicator>(Location_LU + FVector(0, 0, 2), FRotator(0, 0, 0));
	Indicator_Hint = TheWorld->SpawnActor<AChessBoardIndicator>(Location_LU + FVector(0, 0, 2), FRotator(0, 0, 0));
	Indicator_Player->SetIndicatorColor(1);
	Indicator_Last->SetIndicatorColor(2);
	Indicator_Hint->SetIndicatorColor(3);
	Indicator_Player->SetIndicatorVisibility(false);
	Indicator_Last->SetIndicatorVisibility(false);
	Indicator_Hint->SetIndicatorVisibility(false);
	if (!isViewOnBlack)
	{
		std::swap(Location_LU, Location_RD);
	}
	WinX = -1;
	WinY = -1;
	WinDir = -1;
	for (int i = 0; i < Chesses.Num(); i++)
	{
		Chesses[i]->Destroy();
	}
	Chesses = TArray<AActor*>();
	AiSurroundered = false;
	UserSurroundered = false;
	board.clearBoard();
	board.setBanMode(GetBanMode());
	if (AiDifficulty > 2)
	{
		MissFubuki = AiThread();
		MissFubuki.setPlayer(GetSelfPlayer() == 1 ? 2 : 1);
	}
	else
	{
		MissAi.setDifficulty(AiDifficulty);
	}
	
	PlayerHelper.setPlayer(GetSelfPlayer());
	RetractTimeRemain = DefaultRetractTime;
	EnableGameboard();
}

void AChessBoardManager::DisableGameboard()
{
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (Triggers[i][j] != nullptr) Triggers[i][j]->Destroy();
			BoardBuffer[i][j] = false;
		}
	}
	if (Indicator_Last != nullptr) Indicator_Last->SetIndicatorVisibility(false);
	if (Indicator_Player != nullptr) Indicator_Player->SetIndicatorVisibility(false);
	if (Indicator_Hint != nullptr) Indicator_Hint->SetIndicatorVisibility(false);
}

void AChessBoardManager::EnableGameboard()
{
	float boardX = Location_RD.X - Location_LU.X;
	float boardY = Location_RD.Y - Location_LU.Y;
	//float boardZ = Location_RD.Z - Location_LU.Z;
	unitX = boardX / (BOARD_SIZE - 1);
	unitY = boardY / (BOARD_SIZE - 1);
	//float unitZ = boardZ / BOARD_SIZE;
	UWorld* TheWorld = GetWorld();
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("Processing grid(%d, %d)"), i, j));
			AChessLocationTrigger* NewTrigger = TheWorld->SpawnActor<AChessLocationTrigger>(Location_LU + FVector(unitX * i, unitY * j, 2), FRotator(0, 0, 0));
			//TheWorld->SpawnActor<ATestActor>(Location_LU + FVector(unitX * i, unitY * j, 0), FRotator(0, 0, 0));
			if (NewTrigger != nullptr)
			{
				NewTrigger->InitUtility(FVector2D(unitX - 1, unitY - 1), i, j, this);
				Triggers[i][j] = NewTrigger;
			}
		}
	}
}

void AChessBoardManager::SetPlayerIndicator(int32 X, int32 Y, bool IsOk)
{
	if (Indicator_Player == nullptr) return;
	Indicator_Player->SetActorLocation(FVector(Location_LU.X + unitX * X, Location_LU.Y + unitY * Y, Location_LU.Z + 1.2));
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("Player indicator set to (%s)"), IsOk ? TEXT("White") : TEXT("Orange")));
	Indicator_Player->SetIndicatorColor(IsOk ? 1 : 2);
	Indicator_Player->SetIndicatorVisibility(true);
	PlayerIndPos_X = X;
	PlayerIndPos_Y = Y;
}

void AChessBoardManager::HidePlayerIndicator() {
	if (Indicator_Player == nullptr) return;
	Indicator_Player->SetIndicatorVisibility(false); 
}

void AChessBoardManager::SetHintIndicator(int32 X, int32 Y)
{
	if (Indicator_Hint == nullptr) return;
	Indicator_Hint->SetActorLocation(FVector(Location_LU.X + unitX * X, Location_LU.Y + unitY * Y, Location_LU.Z + 1.1));
	Indicator_Hint->SetIndicatorVisibility(true);
}

void AChessBoardManager::HideHintIndicator() {
	if (Indicator_Hint == nullptr) return;
	Indicator_Hint->SetIndicatorVisibility(false);
}

void AChessBoardManager::DisplayHint()
{
	std::pair<int, int> sug = PlayerHelper.makeAction(board);
	SetHintIndicator(sug.first, sug.second);
}

void AChessBoardManager::SetLastIndicator(int32 X, int32 Y)
{
	if (Indicator_Last == nullptr) return;
	Indicator_Last->SetActorLocation(FVector(Location_LU.X + unitX * X, Location_LU.Y + unitY * Y, Location_LU.Z + 1));
	Indicator_Last->SetIndicatorVisibility(true);
}

void AChessBoardManager::HideLastIndicator() {
	if (Indicator_Last == nullptr) return;
	Indicator_Last->SetIndicatorVisibility(false); 
}

void AChessBoardManager::SetSelfPlayer(bool PlayerIsBlack) 
{
	PlayerHasBlack = PlayerIsBlack; 
	MissAi.setPlayer(PlayerIsBlack ? 2 : 1);	// Set opposite to the player.
}

bool AChessBoardManager::PlaceChess(int32 X, int32 Y, bool IsBlack)
{
	if (!board.isAvailable(X, Y, (IsBlack ? 1 : 2))) return false;
	board.placeChess((IsBlack ? 1 : 2), X, Y);
	AGobangChess* ChessObj = GetWorld()->SpawnActor<AGobangChess>(Location_LU + FVector(unitX * X, unitY * Y, 0), FRotator(0, 0, 0));
	if (ChessObj != nullptr)
	{
		ChessObj->SetChessColor(IsBlack);
		ChessObj->SetChessVisibility(true);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), PlaceChessAudio, Location_LU + FVector(unitX * X, unitY * Y, 1));
	}
	Indicator_Last->SetIndicatorVisibility(true);
	SetLastIndicator(X, Y);
	Chesses.Add(ChessObj);
	if (AiDifficulty > 2) MissFubuki.putChess(X, Y, (IsBlack ? 1 : 2));
	return true;
}

void AChessBoardManager::AiTimerTask()
{
	if (AiWorker.isOver())
	{
		std::pair<int, int> decision = AiWorker.getResult();
		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("Robot placed at (%d, %d), Color %s"), decision.first, decision.second, GetSelfPlayer() == 2 ? TEXT("Black") : TEXT("White")));
		if (!board.isAvailable(decision.first, decision.second, GetSelfPlayer() == 1 ? 2 : 1)) AiSurrounder();
		else if (PlaceChess(decision.first, decision.second, GetSelfPlayer() == 2))
		{
			if (IsOver())
			{
				SetCurrentStatus(4);
			}
			else
			{
				SetCurrentStatus(2);
			}
		}
		GetWorldTimerManager().ClearTimer(AiTimerHandle);
	}
}

void AChessBoardManager::MakeAiAction()
{
	if (AiDifficulty > 2) {
		AiWorker = ThreadAiWorker();
		AiWorker.initParam(&MissFubuki);
		AiWorker.start();
		AiTimerDelegate = FTimerDelegate::CreateUObject(this, &AChessBoardManager::AiTimerTask);
		GetWorldTimerManager().SetTimer(AiTimerHandle, AiTimerDelegate, 0.5, true);
	}
	else
	{
		std::pair<int, int> decision = MissAi.makeAction(board);
		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("Robot placed at (%d, %d), Color %s"), decision.first, decision.second, GetSelfPlayer() == 2 ? TEXT("Black") : TEXT("White")));
		if (!board.isAvailable(decision.first, decision.second, GetSelfPlayer() == 1 ? 2 : 1)) AiSurrounder();
		else if (PlaceChess(decision.first, decision.second, GetSelfPlayer() == 2))
		{
			if (IsOver())
			{
				SetCurrentStatus(4);
			}
			else
			{
				SetCurrentStatus(2);
			}
		}
	}
	
}

void AChessBoardManager::AiSurrounder()
{
	AiSurroundered = true;
	SetCurrentStatus(4);
}

void AChessBoardManager::UserSurrounder()
{
	UserSurroundered = true;
	SetCurrentStatus(4);
}

int32 AChessBoardManager::GetWinner() {
	if (IsAiSurroundered()) return GetSelfPlayer();
	return board.getWinner(); 
}

bool AChessBoardManager::RetractChess()
{
	if (Chesses.Num() < 2 || RetractTimeRemain <= 0 || CurrentStatus != 2) return false;
	RetractTimeRemain--;
	if (AiDifficulty > 2)
	{
		std::pair<int, int> la = board.getLastChess();
		MissFubuki.takeChess(la.first, la.second);
		board.retract();
		la = board.getLastChess();
		MissFubuki.takeChess(la.first, la.second);
		board.retract();
	}
	else
	{
		board.retract();
		board.retract();
	}
	HideLastIndicator();
	Chesses[Chesses.Num() - 1]->Destroy();
	Chesses[Chesses.Num() - 2]->Destroy();
	Chesses.RemoveAt(Chesses.Num() - 1);
	Chesses.RemoveAt(Chesses.Num() - 1);
	if (Chesses.Num() > 0)
	{
		std::pair<int, int> las = board.getLastChess();
		SetLastIndicator(las.first, las.second);
	}
	return true;
}

int32 AChessBoardManager::GetWinPositionX()
{
	if (WinX == -1)
	{
		std::pair<int, std::pair<int, int> > res = board.getWinLocation();
		WinX = res.second.first;
		WinY = res.second.second;
		WinDir = res.first;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("X %d"), WinX));
	return WinX;
}

int32 AChessBoardManager::GetWinPositionY()
{
	if (WinY == -1)
	{
		std::pair<int, std::pair<int, int> > res = board.getWinLocation();
		WinX = res.second.first;
		WinY = res.second.second;
		WinDir = res.first;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("Y %d"), WinY));
	return WinY;
}

int32 AChessBoardManager::GetWinPositionDir()
{
	if (WinDir == -1)
	{
		std::pair<int, std::pair<int, int> > res = board.getWinLocation();
		WinX = res.second.first;
		WinY = res.second.second;
		WinDir = res.first;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("DIR %d"), WinDir));
	return WinDir;
}

FVector AChessBoardManager::GetAbsolutePosition(int32 X, int32 Y)
{
	return FVector(Location_LU.X + unitX * X, Location_LU.Y + unitY * Y, Location_LU.Z + 1);
}

// Called when the game starts or when spawned
void AChessBoardManager::BeginPlay()
{
	Super::BeginPlay();

	DefaultRetractTime = RetractTimeRemain;
}

// Called every frame
void AChessBoardManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

