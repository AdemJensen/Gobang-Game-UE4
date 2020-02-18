// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.


#include "GobangGameModeBase.h"

AGobangGameModeBase::AGobangGameModeBase()
{
	// no pawn by default
	DefaultPawnClass = AGobangPlayerPawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = AGobangPlayerController::StaticClass();
}

void AGobangGameModeBase::BeginPlay()
{
	TArray<AActor*> Managers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABoardManagerBase::StaticClass(), Managers);

	if (Managers.Num() != 0)
	{
		Manager = Cast<ABoardManagerBase>(Managers[0]);
	}

}
