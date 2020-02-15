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
		AChessBoardIndicator* Indicator_Hint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Indicators")
		USoundWave* PlaceChessAudio;

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void InitGameBoard(bool isViewOnBlack);

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void DisableGameboard();

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void EnableGameboard();

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void SetPlayerIndicator(int32 X, int32 Y, bool IsOk);

	int32 PlayerIndPos_X = 0;
	int32 PlayerIndPos_Y = 0;

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void SetHintIndicator(int32 X, int32 Y);

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void HideHintIndicator();

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void DisplayHint();

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

	bool AiSurroundered = false;
	UFUNCTION(BlueprintCallable, Category = "Game Operations")
		void AiSurrounder();
	UFUNCTION(BlueprintCallable, Category = "Game Operations")
		bool IsAiSurroundered() { return AiSurroundered; }

	bool UserSurroundered = false;
	UFUNCTION(BlueprintCallable, Category = "Game Operations")
		void UserSurrounder();
	UFUNCTION(BlueprintCallable, Category = "Game Operations")
		bool IsUserSurroundered() { return UserSurroundered; }

	int32 AiDifficulty = 2;
	UFUNCTION(BlueprintCallable, Category = "Game Operations")
		void SetAiDifficulty(int Difficulty) { AiDifficulty = Difficulty; }
	UFUNCTION(BlueprintCallable, Category = "Game Operations")
		int32 GetAiDifficulty() { return AiDifficulty; }

	UFUNCTION(BlueprintCallable, Category = "Game Operations")
		int32 GetWinner();

	UFUNCTION(BlueprintCallable, Category = "Game Operations")
		bool RetractChess();

	UFUNCTION(BlueprintCallable, Category = "Game Operations")
		int32 GetRetractRemain() { return RetractTimeRemain; }

	UFUNCTION(BlueprintCallable, Category = "Game Operations")
		int32 GetChessNum() { return Chesses.Num(); }

	bool useBanMode;
	UFUNCTION(BlueprintCallable, Category = "Game Operations")
		void SetBanMode(bool useBan) { useBanMode = useBan; }
	UFUNCTION(BlueprintCallable, Category = "Game Operations")
		bool GetBanMode() { return useBanMode; }

	UFUNCTION(BlueprintCallable, Category = "Game Operations")
		bool IsAvailable(int X, int Y, int player) { return board.isAvailable(X, Y, player); }

	int32 WinX = -1;
	int32 WinY = -1;
	int32 WinDir = -1;
	UFUNCTION(BlueprintCallable, Category = "Game Operations")
		int32 GetWinPositionX();
	UFUNCTION(BlueprintCallable, Category = "Game Operations")
		int32 GetWinPositionY();
	UFUNCTION(BlueprintCallable, Category = "Game Operations")
		int32 GetWinPositionDir();

	UFUNCTION(BlueprintCallable, Category = "Information")
		FVector GetAbsolutePosition(int32 X, int32 Y);

	UPROPERTY()
		TArray<AActor*> Chesses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
		int32 RetractTimeRemain;

	int32 DefaultRetractTime;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	Board board;

	KizunaAi MissAi;
	KizunaAi PlayerHelper;

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
