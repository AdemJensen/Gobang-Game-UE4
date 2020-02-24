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
	switch (MyGameMode->GameManager->PublicManager->GetGameStage())
	{
	case EGameStage::WAIT_FOR_ACTION:
		RefreshUiAction.Broadcast(MyGameMode->GameManager->GetGamePlayer(MyGameMode->GameManager->PublicManager->GetCurrentPlayer()), TimeRemain);
		break;
	case EGameStage::ROUND_OVER:
		// Logics to judge win or lose
		OnRoundOver();
		break;
	case EGameStage::SURROUNDERED:

		break;
	case EGameStage::RETRACTED:

		break;
	//default:
		// Error.
	}
	
}

void AGameThreadTicker::OnTimeUp()
{
	AGobangGameModeBase* MyGameMode = Cast<AGobangGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	TimeUpAction.Broadcast(MyGameMode->GameManager->GetGamePlayer(MyGameMode->GameManager->PublicManager->GetCurrentPlayer()));
}

void AGameThreadTicker::OnRoundOver()
{
	AGobangGameModeBase* MyGameMode = Cast<AGobangGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	RoundOverAction.Broadcast(MyGameMode->GameManager->GetGamePlayer(MyGameMode->GameManager->PublicManager->GetCurrentPlayer()));
}
