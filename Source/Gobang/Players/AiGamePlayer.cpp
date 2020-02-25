// This is a private project, cannot be put at commercial use.


#include "AiGamePlayer.h"
#include "KizunaAiThreadWorker.h"
#include "FubukiThreadWorker.h"
#include "../GobangGameModeBase.h"
#include "../Basics/ChessType.h"

void AAiGamePlayer::OnGameStart()
{
	if (GetAiLevel() == 3)
	{
		MissFu.setPlayer(GetChessType() == EChessType::BLACK ? 1 : 2);
	}
	else
	{
		MissAi.setDifficulty(GetAiLevel());
		MissAi.setPlayer(GetChessType() == EChessType::BLACK ? 1 : 2);
	}
}


void AAiGamePlayer::OnRoundStart()
{
	Worker = nullptr;
	if (GetAiLevel() == 3)
	{
		Worker = TSharedPtr<FThreadWorkerBase>(new FFubukiThreadWorker());
	}
	else
	{
		Worker = TSharedPtr<FThreadWorkerBase>(new FKizunaAiThreadWorker());
	}
	Worker->Start();
	AiTimerDelegate = FTimerDelegate::CreateUObject(this, &AAiGamePlayer::AiTimerTask);
	GetWorldTimerManager().SetTimer(AiTimerHandle, AiTimerDelegate, 1.0f, true);
}

void AAiGamePlayer::OnInterrupt()
{
	if (Worker.IsValid())
	{
		Worker.Get()->End();
	}
}

void AAiGamePlayer::OnRetract(FIntPoint RetractPosition1, FIntPoint RetractPosition2)
{
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
			Decision = StaticCastSharedPtr<FFubukiThreadWorker>(Worker).Get()->GetResult();
		}
		else
		{
			Decision = StaticCastSharedPtr<FKizunaAiThreadWorker>(Worker).Get()->GetResult();
		}
		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("Robot placed at (%d, %d), Color %s"), decision.first, decision.second, GetSelfPlayer() == 2 ? TEXT("Black") : TEXT("White")));
		AGameModeBase* GameMode = GetGameMode();
		if (GameMode == nullptr) return;
		AGobangGameModeBase* MyGameMode = Cast<AGobangGameModeBase>(GameMode);
		if (MyGameMode == nullptr) return;
		Board* board = MyGameMode->GameManager->BoardManager->GetBoardObj();
		if (!board->isAvailable(Decision.first, Decision.second, GetChessType() == EChessType::BLACK ? 1 : 2)) DoSurrounder();
		else
		{
			if (GetAiLevel() == 3) MissFu.putChess(Decision.first, Decision.second, GetChessType() == EChessType::BLACK ? 1 : 2);
			DoRoundOver(FIntPoint(Decision.first, Decision.second));
		}
		GetWorldTimerManager().ClearTimer(AiTimerHandle);
	}
}