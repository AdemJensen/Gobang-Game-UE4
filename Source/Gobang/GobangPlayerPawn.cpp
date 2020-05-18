// Fill out your copyright notice in the Description page of Project Settings.


#include "GobangPlayerPawn.h"
#include "GobangGameModeBase.h"
#include "Basics/BoardTrigger.h"
#include "Basics/ProgramStage.h"
#include "Basics/BanMode.h"
#include "Players/GamePlayerType.h"

// Sets default values
AGobangPlayerPawn::AGobangPlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGobangPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

#if PLATFORM_WINDOWS || PLATFORM_MAC || PLATFORM_LINUX
void AGobangPlayerPawn::TraceForBlock(const FVector& Start, const FVector& End, bool bDrawDebugHelpers)
{
	FHitResult HitResult;
	UWorld* TheWorld = GetWorld();
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(TheWorld);
	AGobangGameModeBase* MyGameMode = Cast<AGobangGameModeBase>(GameMode);
	if (MyGameMode->GameManager->GetProgramStage() != EProgramStage::IN_GAME) return;
	TheWorld->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
	if (bDrawDebugHelpers)	// Onlu used via VR.
	{
		DrawDebugLine(TheWorld, Start, HitResult.Location, FColor::Red);
		DrawDebugSolidBox(TheWorld, HitResult.Location, FVector(20.0f), FColor::Red);
	}
	if (HitResult.Actor.IsValid())
	{
		AActor* HitActor = HitResult.Actor.Get();
		ABoardTrigger* BoardIndicator = Cast<ABoardTrigger>(HitActor);
		
		if (BoardIndicator != nullptr)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, FString::Printf(TEXT("Now mouse on %s"), (MyGameMode->Manager->IsAvailable(BoardIndicator->Coordinate_X, BoardIndicator->Coordinate_Y, MyGameMode->Manager->GetSelfPlayer()) ? TEXT("Avail") : TEXT("Not avail"))));
			if (MyGameMode->GameManager->GetGamePlayer(MyGameMode->GameManager->PublicManager->GetCurrentPlayer())->GetPlayerType() == EGamePlayerType::LOCAL_PLAYER
				|| MyGameMode->GameManager->GetGamePlayer(MyGameMode->GameManager->PublicManager->GetCurrentPlayer())->GetPlayerType() == EGamePlayerType::LOCAL_REMOTE_PLAYER)
			{
				int Avail = MyGameMode->GameManager->BoardManager->IsAvailable(
					BoardIndicator->Coordinate_X, BoardIndicator->Coordinate_Y,
					MyGameMode->GameManager->PublicManager->GetCurrentPlayer()
				);
				MyGameMode->GameManager->IndicationManager->SetPlayerIndicator(
					BoardIndicator->Coordinate_X, BoardIndicator->Coordinate_Y, 
					(MyGameMode->GameManager->PublicManager->GetBanMode() != EBanMode::ON_ILLEGAL_BANNED || Avail == 0) && Avail != 1
				);

			}
		}
		else
		{
			MyGameMode->GameManager->IndicationManager->HidePlayerIndicator();
		}
	}
}
#else
void AGobangPlayerPawn::TraceForBlock(const FVector& Start, const FVector& End, bool bDrawDebugHelpers) { }
#endif

// Called every frame
void AGobangPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		FVector Start, Dir, End;
		PC->DeprojectMousePositionToWorld(Start, Dir);
		End = Start + (Dir * 8000.0f);
		TraceForBlock(Start, End, false);
	}
}

// Called to bind functionality to input
void AGobangPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//PlayerInputComponent->BindAction("TriggerClick", EInputEvent::IE_Pressed, this, &AGobangPlayerPawn::TriggerClick);
}

