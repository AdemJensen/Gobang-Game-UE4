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
	AActor* Actor = UGameplayStatics::GetActorOfClass(GetWorld(), AGameManagerBase::StaticClass());
	if (Actor != nullptr) GameManager = Cast<AGameManagerBase>(Actor);

}
