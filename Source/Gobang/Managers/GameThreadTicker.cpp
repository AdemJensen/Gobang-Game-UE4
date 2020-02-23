// This is a private project, cannot be put at commercial use.


#include "GameThreadTicker.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "../GobangGameModeBase.h"
#include "../Basics/ChessType.h"

void AGameThreadTicker::OnRemainChanged(float TimeRemain)
{
	AGobangGameModeBase* MyGameMode = Cast<AGobangGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	RefreshUiAction.Broadcast(MyGameMode->GameManager->GetGamePlayer(MyGameMode->GameManager->PublicManager->GetCurrentPlayer()), TimeRemain);
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
