// This is a private project, cannot be put at commercial use.


#include "CriticalRetractDownCounter.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "../GobangGameModeBase.h"
#include "../Basics/ChessType.h"
#include "../Basics/GameStage.h"
#include "GameManagerBase.h"
#include "Engine.h"

void ACriticalRetractDownCounter::StartFinalDowncount() 
{ 
	StartDowncount(5, 0.1); 
}

void ACriticalRetractDownCounter::OnRemainChanged(float TimeRemain)
{
	RefreshUiAction.Broadcast(TimeRemain);
}

void ACriticalRetractDownCounter::OnTimeUp()
{
	AGobangGameModeBase* MyGameMode = Cast<AGobangGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	//TimeUpAction.Broadcast(MyGameMode->GameManager->GetGamePlayer(MyGameMode->GameManager->PublicManager->GetCurrentPlayer()));
	MyGameMode->GameManager->PublicManager->SetGameStage(EGameStage::GAME_OVER);
	MyGameMode->GameManager->SetProgramStage(EProgramStage::AT_RESULT_UI);
	GameEndAction.Broadcast(EndGameReason);
	this->Interrupt();
}
