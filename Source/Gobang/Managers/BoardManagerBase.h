// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Basics/BoardLocator.h"
#include "../Basics/ChessType.h"
#include "../GobangFramework/Board.h"
#include "../GobangFramework/KizunaAi/kizunaAi.h"
#include "../GobangFramework/Fubuki/aithread.h"
#include "Engine.h"
#include "../Basics/BoardIndicator.h"
#include "../Basics/ChessType.h"
#include "../Utilities/ThreadAiWorker.h"
#include "BoardManagerBase.generated.h"

UCLASS()
class GOBANG_API ABoardManagerBase : public AActor
{
	GENERATED_BODY()
	
public:	

	#define BOARD_SIZE 19

	// Sets default values for this actor's properties
	ABoardManagerBase();
	~ABoardManagerBase();

	UFUNCTION(BlueprintCallable, Category = "Game Actions")	
		void InitGameBoard(ABoardLocator* LU, ABoardLocator* RD, bool bBanModeOn);

	/*
	Place a chess on the world board.
		0: Success.
		1: Failed (Board Taken).
		2: Banned position.
	*/
	UFUNCTION(BlueprintCallable, Category = "Game Actions")
		int PlaceChess(int32 X, int32 Y, bool IsBlack);

	UFUNCTION(BlueprintCallable, Category = "Game Info")
		bool IsOver() { return board.isOver(); }

	/*
	The winner can only be:
		0: None.
		1: Player 1.
		2: Player 2.
		-1: Board full.
	 */
	UFUNCTION(BlueprintCallable, Category = "Game Info")
		int32 GetWinner();

	UFUNCTION(BlueprintCallable, Category = "Game Actions")
		bool RetractChess();

	UFUNCTION(BlueprintCallable, Category = "Game Info")
		int32 GetChessNum() { return Chesses.Num(); }

	bool bUseBanMode;	// Balance breaker.
	UFUNCTION(BlueprintCallable, Category = "Game Info")
		bool GetBanMode() { return bUseBanMode; }

	UFUNCTION(BlueprintCallable, Category = "Game Info")
		int IsAvailable(int X, int Y, EChessType ChessType) { return board.isAvailable(X, Y, ChessType == EChessType::BLACK ? 1 : 2); }

	FIntPoint WinPos;
	int32 WinDir = -1;
	UFUNCTION(BlueprintCallable, Category = "Game Info")
		FIntPoint GetWinPosition();
	UFUNCTION(BlueprintCallable, Category = "Game Info")
		int32 GetWinPositionDir();

	// Get the absoulte location in the world using chess coordinate.
	UFUNCTION(BlueprintCallable, Category = "Game Info")
		FVector GetAbsolutePosition(int32 X, int32 Y, float Z_Offset = 0);

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	Board board;

	// Tirggers on the board to place chess.
	AActor* Triggers[BOARD_SIZE][BOARD_SIZE];

	FVector Location_LU;
	FVector Location_RD;

	// Used to convert Board coordinates to World locations.
	float unitX;
	float unitY;

	UPROPERTY()
		TArray<AActor*> Chesses;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
