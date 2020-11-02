// This is a private project, cannot be put at commercial use.


#include "AiGamePlayer.h"
#include "KizunaAiThreadWorker.h"
#include "FubukiThreadWorker.h"
#include "../GobangGameModeBase.h"
#include "../Basics/ChessType.h"
#include "Engine.h"

void AAiGamePlayer::OnGameStart()
{
	Super::OnGameStart();
	Worker = nullptr;
	if (GetAiLevel() == 3)
	{
		MissFu.setPlayer(GetChessType() == EChessType::BLACK ? 1 : 2);
		TSharedPtr<FFubukiThreadWorker> Temp = TSharedPtr<FFubukiThreadWorker>(new FFubukiThreadWorker());
		Temp->InitParam(&MissFu);
		Worker = StaticCastSharedPtr<FThreadWorkerBase>(Temp);
	}
	else
	{
		MissAi.setDifficulty(GetAiLevel());
		MissAi.setPlayer(GetChessType() == EChessType::BLACK ? 1 : 2);
		TSharedPtr<FKizunaAiThreadWorker> Temp = TSharedPtr<FKizunaAiThreadWorker>(new FKizunaAiThreadWorker());
		Temp->InitParam(&MissAi, Cast<AGobangGameModeBase>(GetGameMode())->GameManager->BoardManager->GetBoardObj());
		Worker = StaticCastSharedPtr<FThreadWorkerBase>(Temp);
	}
}


void AAiGamePlayer::OnRoundStart()
{
	Super::OnRoundStart();
	//*DEBUG*/GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("Ai Got round start info."));
	Worker->Start();
	AiTimerDelegate = FTimerDelegate::CreateUObject(this, &AAiGamePlayer::AiTimerTask);
	GetWorldTimerManager().SetTimer(AiTimerHandle, AiTimerDelegate, 1.0f, true);
}

void AAiGamePlayer::OnInterrupt()
{
	Super::OnInterrupt();
	if (Worker.IsValid())
	{
		Worker.Get()->End();
	}
}

void AAiGamePlayer::OnRetract(FIntPoint RetractPosition1, FIntPoint RetractPosition2)
{
	Super::OnRetract(RetractPosition1, RetractPosition2);
	if (GetAiLevel() == 3)
	{
		MissFu.takeChess(RetractPosition1.X, RetractPosition1.Y);
		MissFu.takeChess(RetractPosition2.X, RetractPosition2.Y);
	}
}

void AAiGamePlayer::AiTimerTask()
{
	if (Worker.Get()->IsOver())
	{
		std::pair<int, int> Decision;
		
		if (GetAiLevel() == 3)
		{
			Decision = StaticCastSharedPtr<FFubukiThreadWorker>(Worker)->GetResult();
		}
		else
		{
			Decision = StaticCastSharedPtr<FKizunaAiThreadWorker>(Worker)->GetResult();
		}
		//*DEBUG*/GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("Ai Got the work done (%d, %d)."), Decision.first, Decision.second));
		AGameModeBase* GameMode = GetGameMode();
		if (GameMode == nullptr) return;
		AGobangGameModeBase* MyGameMode = Cast<AGobangGameModeBase>(GameMode);
		if (MyGameMode == nullptr) return;
		Board* board = MyGameMode->GameManager->BoardManager->GetBoardObj();
		if (board->isAvailable(Decision.first, Decision.second, GetChessType() == EChessType::BLACK ? 1 : 2) != 0) DoSurrounder();
		else
		{
			if (GetAiLevel() == 3) MissFu.putChess(Decision.first, Decision.second, GetChessType() == EChessType::BLACK ? 1 : 2);
			DoRoundOver(FIntPoint(Decision.first, Decision.second));
		}
		GetWorldTimerManager().ClearTimer(AiTimerHandle);
	}
}