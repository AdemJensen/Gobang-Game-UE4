// This is a private project, cannot be put at commercial use.


#include "GameThreadTicker.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "../GobangGameModeBase.h"
#include "../Basics/ChessType.h"
#include "../Basics/GameStage.h"
#include "GameManagerBase.h"
#include "Engine.h"

void AGameThreadTicker::OnRemainChanged(float TimeRemain)
{

	AGobangGameModeBase* MyGameMode = Cast<AGobangGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	AGamePlayerBase* Player = MyGameMode->GameManager->GetGamePlayer(MyGameMode->GameManager->PublicManager->GetCurrentPlayer());
	std::pair<FIntPoint, FIntPoint> RetractResult;
	int32 BoardResult;
	//*DEBUG*/GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("Master Thread is running."));
	switch (MyGameMode->GameManager->PublicManager->GetGameStage())
	{
	case EGameStage::UNKNOWN:	// Start the engine.
		//*DEBUG*/GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("Got Unknown."));
		RoundOverAction.Broadcast(MyGameMode->GameManager->GetGamePlayer(EChessType::BLACK));
		MyGameMode->GameManager->PublicManager->SetCurrentPlayer(EChessType::BLACK);
		MyGameMode->GameManager->PublicManager->SetGameStage(EGameStage::WAIT_FOR_ACTION);
		MyGameMode->GameManager->GetGamePlayer(EChessType::BLACK)->OnGameStart();
		MyGameMode->GameManager->GetGamePlayer(EChessType::WHITE)->OnGameStart();
		MyGameMode->GameManager->GetGamePlayer(EChessType::BLACK)->OnRoundStart();
		break;
	case EGameStage::WAIT_FOR_ACTION:
		RefreshUiAction.Broadcast(Player, TimeRemain);
		break;
	case EGameStage::ROUND_OVER:
		// Logics to judge win or lose
		if (MyGameMode->GameManager->BoardManager->PlaceChess(ActionPoint.X, ActionPoint.Y, Player->GetChessType()) != 0)	// Ilegal action.
		{
			//*DEBUG*/GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("Ilegal action."));
			if (Player->GetChessType() != EChessType::BLACK)	// Only black chess can have balance breakers.
			{
				OnGameOver(EEndGameReason::BLACK_ERROR);
			}
			else
			{
				if (MyGameMode->GameManager->GetGamePlayer(EChessType::WHITE)->GetRetractRemainTimes() != 0)
				{
					MyGameMode->GameManager->PublicManager->SetGameStage(EGameStage::ILEGAL_ACTION);
					OnCritical(MyGameMode->GameManager->PublicManager->GetCurrentPlayer() == EChessType::BLACK ? EEndGameReason::BLACK_ILEGAL_ACTION : EEndGameReason::WHITE_ILEGAL_ACTION);
					return;
				}
				else
				{
					OnGameOver(MyGameMode->GameManager->PublicManager->GetCurrentPlayer() == EChessType::BLACK ? EEndGameReason::BLACK_ILEGAL_ACTION : EEndGameReason::WHITE_ILEGAL_ACTION);
					return;
				}
				this->Interrupt();
			}
		}
		BoardResult = MyGameMode->GameManager->BoardManager->GetWinner();
		switch (BoardResult)
		{
		case 0: // None
			//*DEBUG*/GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("normal round over."));
			OnRoundOver();
			break;
		case 1: // Black win
			//*DEBUG*/GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("Black win."));
			if (Player->GetChessType() != EChessType::BLACK)
			{
				OnGameOver(EEndGameReason::BLACK_ERROR);
			}
			else
			{
				if (MyGameMode->GameManager->GetGamePlayer(EChessType::WHITE)->GetRetractRemainTimes() != 0)
				{
					MyGameMode->GameManager->PublicManager->SetGameStage(EGameStage::CHECKMATE);
					OnCritical(EEndGameReason::BLACK_APPEND_5);
					this->Interrupt();
				}
				else
				{
					OnGameOver(EEndGameReason::BLACK_APPEND_5);
				}
			}
			break;
		case 2: // White win
			//*DEBUG*/GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("White win."));
			if (Player->GetChessType() != EChessType::WHITE)
			{
				OnGameOver(EEndGameReason::WHITE_ERROR);
			}
			else
			{
				if (MyGameMode->GameManager->GetGamePlayer(EChessType::BLACK)->GetRetractRemainTimes() != 0)
				{
					MyGameMode->GameManager->PublicManager->SetGameStage(EGameStage::CHECKMATE);
					OnCritical(EEndGameReason::WHITE_APPEND_5);
					this->Interrupt();
				}
				else
				{
					OnGameOver(EEndGameReason::WHITE_APPEND_5);
				}
			}
			break;
		case -1: // Board full
			OnGameOver(EEndGameReason::DRAW_BOARD_FULL);
			break;
		}
		break;
	case EGameStage::SURROUNDERED:
		OnGameOver(MyGameMode->GameManager->PublicManager->GetCurrentPlayer() == EChessType::BLACK ? EEndGameReason::BLACK_SURROUNDER : EEndGameReason::WHITE_SURROUNDER);
		return;
	case EGameStage::RETRACTED:
		//*DEBUG*/GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, TEXT("Retract Mode."));

		if (Player->GetRetractRemainTimes() == 0)
		{
			// Error
			OnGameOver(MyGameMode->GameManager->PublicManager->GetCurrentPlayer() == EChessType::BLACK ? EEndGameReason::BLACK_ERROR : EEndGameReason::WHITE_ERROR);
			return;
		}
		else
		{
			if (Player->GetRetractRemainTimes() > 0) Player->SetRetractRemainTimes(Player->GetRetractRemainTimes() - 1);
			RetractResult = MyGameMode->GameManager->BoardManager->RetractChess();
			if (RetractResult.second.X < 0)
			{
				OnGameOver(MyGameMode->GameManager->PublicManager->GetCurrentPlayer() == EChessType::BLACK ? EEndGameReason::BLACK_ERROR : EEndGameReason::WHITE_ERROR);
				return;
			}
			else
			{
				MyGameMode->GameManager->GetGamePlayer(EChessType::BLACK)->OnRetract(RetractResult.first, RetractResult.second);
				MyGameMode->GameManager->GetGamePlayer(EChessType::WHITE)->OnRetract(RetractResult.first, RetractResult.second);
				RetractAction.Broadcast(Player);
				MyGameMode->GameManager->PublicManager->SetGameStage(EGameStage::WAIT_FOR_ACTION);
			}
			FIntPoint LastLoc = MyGameMode->GameManager->BoardManager->GetLastPosition();
			if (LastLoc.X > 0 && LastLoc.Y > 0) MyGameMode->GameManager->IndicationManager->SetLastIndicator(LastLoc.X, LastLoc.Y);
			else MyGameMode->GameManager->IndicationManager->HideLastIndicator();
		}
		break;
	default:
		OnGameOver(MyGameMode->GameManager->PublicManager->GetCurrentPlayer() == EChessType::BLACK ? EEndGameReason::BLACK_ERROR : EEndGameReason::WHITE_ERROR);
		return;
	}
	
}

void AGameThreadTicker::OnGameOver(EEndGameReason Reason)
{
	AGobangGameModeBase* MyGameMode = Cast<AGobangGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	MyGameMode->GameManager->PublicManager->SetGameStage(EGameStage::GAME_OVER);
	MyGameMode->GameManager->SetProgramStage(EProgramStage::AT_RESULT_UI);
	MyGameMode->GameManager->SetReviewData(MyGameMode->GameManager->BoardManager->GetReviewData());
	GameEndAction.Broadcast(Reason);
	this->Interrupt();
}

void AGameThreadTicker::OnTimeUp()
{
	AGobangGameModeBase* MyGameMode = Cast<AGobangGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	//TimeUpAction.Broadcast(MyGameMode->GameManager->GetGamePlayer(MyGameMode->GameManager->PublicManager->GetCurrentPlayer()));
	MyGameMode->GameManager->PublicManager->SetGameStage(EGameStage::TIME_UP);
	MyGameMode->GameManager->SetProgramStage(EProgramStage::AT_RESULT_UI);
	GameEndAction.Broadcast(MyGameMode->GameManager->PublicManager->GetCurrentPlayer() == EChessType::BLACK ? EEndGameReason::BLACK_TIME_UP : EEndGameReason::WHITE_TIME_UP);
	this->Interrupt();
}

void AGameThreadTicker::Interrupt()
{
	Super::Interrupt();
	/*RoundOverAction.RemoveAll(this);
	RetractAction.RemoveAll(this);
	GameEndAction.RemoveAll(this);
	RefreshUiAction.RemoveAll(this);*/
}

void AGameThreadTicker::OnCritical(EEndGameReason Reason)
{
	AGobangGameModeBase* MyGameMode = Cast<AGobangGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	EChessType NextType = MyGameMode->GameManager->PublicManager->GetCurrentPlayer() == EChessType::BLACK ? EChessType::WHITE : EChessType::BLACK;
	MyGameMode->GameManager->PublicManager->SetCurrentPlayer(NextType);
	MyGameMode->GameManager->SetReviewData(MyGameMode->GameManager->BoardManager->GetReviewData());
	CriticalAction.Broadcast(Reason);
}

void AGameThreadTicker::OnRoundOver()
{
	//*DEBUG*/GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("OnRoundOver() Called."));
	AGobangGameModeBase* MyGameMode = Cast<AGobangGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	EChessType NextType = MyGameMode->GameManager->PublicManager->GetCurrentPlayer() == EChessType::BLACK ? EChessType::WHITE : EChessType::BLACK;
	MyGameMode->GameManager->PublicManager->SetCurrentPlayer(NextType);
	RoundOverAction.Broadcast(MyGameMode->GameManager->GetGamePlayer(NextType));
	MyGameMode->GameManager->PublicManager->SetGameStage(EGameStage::WAIT_FOR_ACTION);
	MyGameMode->GameManager->GetGamePlayer(NextType)->OnRoundStart();
	this->RestartDowncount();
}
