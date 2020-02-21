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

void AGamePlayerBase::OnRoundOver()
{
	AGameModeBase* GameMode = GetGameMode();
	if (GameMode == nullptr) return;
	AGobangGameModeBase* MyGameMode = Cast<AGobangGameModeBase>(GameMode);
	if (MyGameMode == nullptr) return;
	MyGameMode->GameManager->OnRoundOver();
}

void AGamePlayerBase::OnSurrounder()
{
	AGameModeBase* GameMode = GetGameMode();
	if (GameMode == nullptr) return;
	AGobangGameModeBase* MyGameMode = Cast<AGobangGameModeBase>(GameMode);
	if (MyGameMode == nullptr) return;
	MyGameMode->GameManager->OnSurrounder(GetChessType());
}

void AGamePlayerBase::OnUnexpectedAction(EUnexpectedGameActionType Type)
{
	AGameModeBase* GameMode = GetGameMode();
	if (GameMode == nullptr) return;
	AGobangGameModeBase* MyGameMode = Cast<AGobangGameModeBase>(GameMode);
	if (MyGameMode == nullptr) return;
	MyGameMode->GameManager->OnUnexpectedAction(Type);
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

