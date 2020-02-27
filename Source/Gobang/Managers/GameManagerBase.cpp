// This is a private project, cannot be put at commercial use.


#include "GameManagerBase.h"
#include "../Basics/Chess.h"
#include "../Basics/GameStage.h"

// Sets default values
AGameManagerBase::AGameManagerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AGameManagerBase::DoPrepareEnv()
{
	UWorld* TheWorld = GetWorld();
	if (TheWorld == nullptr) return;

	/*AActor* Temp = UGameplayStatics::GetActorOfClass(TheWorld, ABoardManagerBase::StaticClass());
	if (Temp == nullptr) return 2;
	BoardManager = Cast<ABoardManagerBase>(Temp);
	if (BoardManager == nullptr) return 2;*/

	/*Temp = UGameplayStatics::GetActorOfClass(TheWorld, APublicManagerBase::StaticClass());
	if (Temp == nullptr) return 3;
	PublicManager = Cast<APublicManagerBase>(Temp);
	if (PublicManager == nullptr) return 3;*/

	TArray<AActor*> TempArr;
	UGameplayStatics::GetAllActorsOfClass(TheWorld, AChess::StaticClass(), TempArr);
	for (int i = 0; i < TempArr.Num(); i++) TempArr[i]->Destroy();

	GameThread = TheWorld->SpawnActor<AGameThreadTicker>(FVector(0, 0, 0), FRotator(0, 0, 0));
}

/*
Prepare the parameters for gameplay.
*/
void AGameManagerBase::DoGameStart()
{
	UWorld* TheWorld = GetWorld();
	if (TheWorld == nullptr) return;

	GamePlayer_Black->SetRetractRemainTimes(PublicManager->GetTotalRetractTimes());
	GamePlayer_White->SetRetractRemainTimes(PublicManager->GetTotalRetractTimes());

	// Start Master Engine
	GameThread->SetTimeLimit(PublicManager->GetRoundTimeLimit() == 0.0f ? -1 : PublicManager->GetRoundTimeLimit());
	GameThread->StartGameThread();
}



void AGameManagerBase::DoRoundOver(FIntPoint ActionLocation)
{
	if (PublicManager->GetGameStage() == EGameStage::WAIT_FOR_ACTION)
	{
		IndicationManager->PlayChessSound(true, IndicationManager->GetAbsolutePosition(ActionLocation.X, ActionLocation.Y));
		IndicationManager->SetLastIndicator(ActionLocation.X, ActionLocation.Y);
		GameThread->SetActionPoint(ActionLocation);
		PublicManager->SetGameStage(EGameStage::ROUND_OVER);
	}
}

void AGameManagerBase::DoSurrounder(EChessType SurrounderTarget)
{
	if (PublicManager->GetGameStage() == EGameStage::WAIT_FOR_ACTION)
	{
		PublicManager->SetGameStage(EGameStage::SURROUNDERED);
	}
}

void AGameManagerBase::DoUnexpectedAction(EUnexpectedGameActionType Type)
{
	if (PublicManager->GetGameStage() == EGameStage::WAIT_FOR_ACTION)
	{
		PublicManager->SetGameStage(EGameStage::UNEXPECTED_ACTION);
		PublicManager->SetUnexpectedActionType(Type);
	}
}

void AGameManagerBase::DoRetract()
{
	if (PublicManager->GetGameStage() == EGameStage::WAIT_FOR_ACTION)
	{
		PublicManager->SetGameStage(EGameStage::RETRACTED);
	}
}

void AGameManagerBase::SetGamePlayer(EChessType PlayerType, AGamePlayerBase* GamePlayer)
{
	if (PlayerType == EChessType::BLACK)
	{
		GamePlayer_Black = GamePlayer;
	}
	else
	{
		GamePlayer_White = GamePlayer;
	}
	GamePlayer->SetChessType(PlayerType);
}

// Called when the game starts or when spawned
void AGameManagerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGameManagerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

