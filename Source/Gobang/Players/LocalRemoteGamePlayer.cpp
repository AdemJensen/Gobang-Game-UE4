	// This is a private project, cannot be put at commercial use.


#include "LocalRemoteGamePlayer.h"
#include "Engine.h"
#include "KizunaAiThreadWorker.h"
#include "../GobangGameModeBase.h"

void ALocalRemoteGamePlayer::AiTimerTask()
{
	if (Worker.Get()->IsOver())
	{
		std::pair<int, int> Decision = StaticCastSharedPtr<FKizunaAiThreadWorker>(Worker)->GetResult();
		//*DEBUG*/GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("Ai Got the work done (%d, %d)."), Decision.first, Decision.second));
		AGameModeBase* GameMode = GetGameMode();
		if (GameMode == nullptr) return;
		AGobangGameModeBase* MyGameMode = Cast<AGobangGameModeBase>(GameMode);
		if (MyGameMode == nullptr) return;
		Board* board = MyGameMode->GameManager->BoardManager->GetBoardObj();
		if (board->isAvailable(Decision.first, Decision.second, GetChessType() == EChessType::BLACK ? 1 : 2) == 0)
		{
			WorkerDoneDelegate.Broadcast(FIntPoint(Decision.first, Decision.second));
		}
		else
		{
			WorkerDoneDelegate.Broadcast(FIntPoint(-1, -1));
		}
		GetWorldTimerManager().ClearTimer(AiTimerHandle);
		bAiRunning = false;
	}
}

void ALocalRemoteGamePlayer::StartHintCalculation()
{
	Worker->Start();
	AiTimerDelegate = FTimerDelegate::CreateUObject(this, &ALocalRemoteGamePlayer::AiTimerTask);
	GetWorldTimerManager().SetTimer(AiTimerHandle, AiTimerDelegate, 0.5f, true);
}

void ALocalRemoteGamePlayer::OnGameStart()
{
	Super::OnGameStart();
	Worker = nullptr;
	MissAi.setDifficulty(2);
	MissAi.setPlayer(GetChessType() == EChessType::BLACK ? 1 : 2);
	TSharedPtr<FKizunaAiThreadWorker> Temp = TSharedPtr<FKizunaAiThreadWorker>(new FKizunaAiThreadWorker());
	Temp->InitParam(&MissAi, Cast<AGobangGameModeBase>(GetGameMode())->GameManager->BoardManager->GetBoardObj());
	Worker = StaticCastSharedPtr<FThreadWorkerBase>(Temp);
}

void ALocalRemoteGamePlayer::APPLY_DoRoundOver(FIntPoint ActionLocation)
{
	if (bAiRunning)
	{
		Worker->End();
		GetWorldTimerManager().ClearTimer(AiTimerHandle);
		WorkerDoneDelegate.Broadcast(FIntPoint(-1, -1));
	}
	Super::DoRoundOver(ActionLocation);
	AGameModeBase* GameMode = GetGameMode();
	if (GameMode == nullptr) return;
	AGobangGameModeBase* MyGameMode = Cast<AGobangGameModeBase>(GameMode);
	if (MyGameMode == nullptr) return;
	MyGameMode->GameManager->IndicationManager->HideHintIndicator();
	UE_LOG(LogTemp, Warning, TEXT("LRGP: APPLY OK."));
}
