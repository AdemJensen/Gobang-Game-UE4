// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChessBoardLocator.h"
#include "GobangFramework/Board.h"
#include "GobangFramework/kizunaAi.h"
#include "Engine.h"
#include "ChessBoardIndicator.h"
#include "ChessBoardManager.generated.h"

UCLASS()
class GOBANG_API AChessBoardManager : public AActor
{
	GENERATED_BODY()
	
public:	

	#define BOARD_SIZE 19

	// Sets default values for this actor's properties
	AChessBoardManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Locators")
		AChessBoardLocator* Locator_LU;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Locators")
		AChessBoardLocator* Locator_RD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Indicators")
		AChessBoardIndicator* Indicator_Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Indicators")
		AChessBoardIndicator* Indicator_Last;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Indicators")
		USoundWave* PlaceChessAudio;

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void InitGameBoard(bool isViewOnBlack);

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void DisableGameboard();

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void EnableGameboard();

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void SetPlayerIndicator(int32 X, int32 Y);

	int32 PlayerIndPos_X = 0;
	int32 PlayerIndPos_Y = 0;

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void HidePlayerIndicator();

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void SetLastIndicator(int32 X, int32 Y);

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void HideLastIndicator();

	bool PlayerHasBlack;
	UFUNCTION(BlueprintCallable, Category = "Information")
		int32 GetSelfPlayer() { return PlayerHasBlack ? 1 : 2; }
	UFUNCTION(BlueprintCallable, Category = "Information")
		void SetSelfPlayer(bool PlayerIsBlack);

	int32 CurrentStatus = 0;
	UFUNCTION(BlueprintCallable, Category = "Information")
		int32 GetCurrentStatus() { return CurrentStatus; }
	/*	0 If at first main window.
		1 If at chess selection window.
		2 if at player.
		3 If at Bot.
		4 If end.
	*/
	UFUNCTION(BlueprintCallable, Category = "Information")
		void SetCurrentStatus(int32 Status) { CurrentStatus = Status; }

	UFUNCTION(BlueprintCallable, Category = "Game Operations")
		bool PlaceChess(int32 X, int32 Y, bool IsBlack);

	UFUNCTION(BlueprintCallable, Category = "Game Operations")
		void MakeAiAction();

	UFUNCTION(BlueprintCallable, Category = "Game Operations")
		bool IsOver() { return board.isOver(); }

	UFUNCTION(BlueprintCallable, Category = "Game Operations")
		int32 GetWinner() { return board.getWinner(); }

	int32 WinX = -1;
	int32 WinY = -1;
	int32 WinDir = -1;
	UFUNCTION(BlueprintCallable, Category = "Game Operations")
		int32 GetWinPositionX();
	UFUNCTION(BlueprintCallable, Category = "Game Operations")
		int32 GetWinPositionY();
	UFUNCTION(BlueprintCallable, Category = "Game Operations")
		int32 GetWinPositionDir();

	UFUNCTION(BlueprintCallable, Category = "Informations")
		FVector GetAbsolutePosition(int32 X, int32 Y);

	UPROPERTY()
		TArray<AActor*> Chesses;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	Board board;

	KizunaAi MissAi;

	AActor* Triggers[BOARD_SIZE][BOARD_SIZE];

	bool BoardBuffer[BOARD_SIZE][BOARD_SIZE];

	FVector Location_LU;
	FVector Location_RD;

	float unitX;
	float unitY;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
