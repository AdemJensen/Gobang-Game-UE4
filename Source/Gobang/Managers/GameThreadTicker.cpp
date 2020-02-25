// This is a private project, cannot be put at commercial use.


#include "GameThreadTicker.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "../GobangGameModeBase.h"
#include "../Basics/ChessType.h"
#include "../Basics/GameStage.h"
#include "GameManagerBase.h"

void AGameThreadTicker::OnRemainChanged(float TimeRemain)
{
	AGobangGameModeBase* MyGameMode = Cast<AGobangGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	AGamePlayerBase* Player = MyGameMode->GameManager->GetGamePlayer(MyGameMode->GameManager->PublicManager->GetCurrentPlayer());
	std::pair<FIntPoint, FIntPoint> RetractResult;
	int32 BoardResult;
	switch (MyGameMode->GameManager->PublicManager->GetGameStage())
	{
	case EGameStage::UNKNOWN:
		RoundOverAction.Broadcast(MyGameMode->GameManager->GetGamePlayer(EChessType::BLACK));
		MyGameMode->GameManager->PublicManager->SetCurrentPlayer(EChessType::BLACK);
		MyGameMode->GameManager->PublicManager->SetGameStage(EGameStage::WAIT_FOR_ACTION);
		break;
	case EGameStage::WAIT_FOR_ACTION:
		RefreshUiAction.Broadcast(Player, TimeRemain);
		break;
	case EGameStage::ROUND_OVER:
		// Logics to judge win or lose
		if (MyGameMode->GameManager->BoardManager->IsAvailable(ActionPoint.X, ActionPoint.Y, Player->GetChessType()) != 0)	// Ilegal action.
		{
			GameEndAction.Broadcast(MyGameMode->GameManager->PublicManager->GetCurrentPlayer() == EChessType::BLACK ? EEndGameReason::BLACK_ILEGAL_ACTION : EEndGameReason::WHITE_ILEGAL_ACTION);
			this->Interrupt();
			return;
		}
		BoardResult = MyGameMode->GameManager->BoardManager->GetWinner();
		switch (BoardResult)
		{
		case 0: // None
			OnRoundOver();
			break;
		case 1: // Black win
			if (Player->GetChessType() != EChessType::BLACK) GameEndAction.Broadcast(EEndGameReason::BLACK_ERROR);
			else GameEndAction.Broadcast(EEndGameReason::BLACK_APPEND_5);
			this->Interrupt();
			break;
		case 2: // White win
			if (Player->GetChessType() != EChessType::WHITE) GameEndAction.Broadcast(EEndGameReason::WHITE_ERROR);
			else GameEndAction.Broadcast(EEndGameReason::WHITE_APPEND_5);
			this->Interrupt();
			break;
		case -1: // Board full
			GameEndAction.Broadcast(EEndGameReason::DRAW_BOARD_FULL);
			this->Interrupt();
			break;
		}
		break;
	case EGameStage::SURROUNDERED:
		GameEndAction.Broadcast(MyGameMode->GameManager->PublicManager->GetCurrentPlayer() == EChessType::BLACK ? EEndGameReason::BLACK_SURROUNDER : EEndGameReason::WHITE_SURROUNDER);
		this->Interrupt();
		return;
	case EGameStage::RETRACTED:
		if (Player->GetRetractRemainTimes() == 0)
		{
			// Error
			GameEndAction.Broadcast(MyGameMode->GameManager->PublicManager->GetCurrentPlayer() == EChessType::BLACK ? EEndGameReason::BLACK_ERROR : EEndGameReason::WHITE_ERROR);
			this->Interrupt();
			return;
		}
		else
		{
			if (Player->GetRetractRemainTimes() > 0) Player->SetRetractRemainTimes(Player->GetRetractRemainTimes() - 1);
			RetractResult = MyGameMode->GameManager->BoardManager->RetractChess();
			if (RetractResult.second.X < 0)
			{
				GameEndAction.Broadcast(MyGameMode->GameManager->PublicManager->GetCurrentPlayer() == EChessType::BLACK ? EEndGameReason::BLACK_ERROR : EEndGameReason::WHITE_ERROR);
				this->Interrupt();
				return;
			}
			else
			{
				MyGameMode->GameManager->GetGamePlayer(EChessType::BLACK)->OnRetract(RetractResult.first, RetractResult.second);
				MyGameMode->GameManager->GetGamePlayer(EChessType::WHITE)->OnRetract(RetractResult.first, RetractResult.second);
				RetractAction.Broadcast(Player);
				MyGameMode->GameManager->PublicManager->SetGameStage(EGameStage::WAIT_FOR_ACTION);
			}
			
		}
		break;
	default:
		GameEndAction.Broadcast(MyGameMode->GameManager->PublicManager->GetCurrentPlayer() == EChessType::BLACK ? EEndGameReason::BLACK_ERROR : EEndGameReason::WHITE_ERROR);
		this->Interrupt();
		return;
	}
	
}

void AGameThreadTicker::OnTimeUp()
{
	AGobangGameModeBase* MyGameMode = Cast<AGobangGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	//TimeUpAction.Broadcast(MyGameMode->GameManager->GetGamePlayer(MyGameMode->GameManager->PublicManager->GetCurrentPlayer()));
	GameEndAction.Broadcast(MyGameMode->GameManager->PublicManager->GetCurrentPlayer() == EChessType::BLACK ? EEndGameReason::BLACK_TIME_UP : EEndGameReason::WHITE_TIME_UP);
	this->Interrupt();
}

void AGameThreadTicker::Interrupt()
{
	Super::Interrupt();
	RoundOverAction.RemoveAll(this);
	RetractAction.RemoveAll(this);
	GameEndAction.RemoveAll(this);
	RefreshUiAction.RemoveAll(this);
}

void AGameThreadTicker::OnRoundOver()
{
	AGobangGameModeBase* MyGameMode = Cast<AGobangGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	EChessType NextType = MyGameMode->GameManager->PublicManager->GetCurrentPlayer() == EChessType::BLACK ? EChessType::WHITE : EChessType::BLACK;
	RoundOverAction.Broadcast(MyGameMode->GameManager->GetGamePlayer(NextType));
	MyGameMode->GameManager->PublicManager->SetCurrentPlayer(NextType);
	MyGameMode->GameManager->PublicManager->SetGameStage(EGameStage::WAIT_FOR_ACTION);
	this->RestartDowncount();
}
