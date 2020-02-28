// This is a private project, cannot be put at commercial use.


#include "GamePlayerBase.h"
#include "Engine/World.h"
#include "../GobangGameModeBase.h"


// Sets default values
AGamePlayerBase::AGamePlayerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AGamePlayerBase::DoRoundOver(FIntPoint ActionLocation)
{
	AGameModeBase* GameMode = GetGameMode();
	if (GameMode == nullptr) return;
	AGobangGameModeBase* MyGameMode = Cast<AGobangGameModeBase>(GameMode);
	if (MyGameMode == nullptr) return;
	MyGameMode->GameManager->DoRoundOver(ActionLocation);
}

void AGamePlayerBase::DoSurrounder()
{
	AGameModeBase* GameMode = GetGameMode();
	if (GameMode == nullptr) return;
	AGobangGameModeBase* MyGameMode = Cast<AGobangGameModeBase>(GameMode);
	if (MyGameMode == nullptr) return;
	MyGameMode->GameManager->DoSurrounder(GetChessType());
}

void AGamePlayerBase::DoUnexpectedAction(EUnexpectedGameActionType Type)
{
	AGameModeBase* GameMode = GetGameMode();
	if (GameMode == nullptr) return;
	AGobangGameModeBase* MyGameMode = Cast<AGobangGameModeBase>(GameMode);
	if (MyGameMode == nullptr) return;
	MyGameMode->GameManager->DoUnexpectedAction(Type);
}

void AGamePlayerBase::DoRetract()
{
	AGameModeBase* GameMode = GetGameMode();
	if (GameMode == nullptr) return;
	AGobangGameModeBase* MyGameMode = Cast<AGobangGameModeBase>(GameMode);
	if (MyGameMode == nullptr) return;
	MyGameMode->GameManager->DoRetract();
}

// Called when the game starts or when spawned
void AGamePlayerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

AGameModeBase* AGamePlayerBase::GetGameMode()
{
	UWorld* TheWorld = GetWorld();
	if (TheWorld == nullptr) return nullptr;
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(TheWorld);
	return GameMode;
}

// Called every frame
void AGamePlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

