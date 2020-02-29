// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Basics/BoardLocator.h"
#include "../Basics/ChessType.h"
#include "../GobangFramework/Board.h"
#include "../GobangFramework/KizunaAi/kizunaAi.h"
#include "../GobangFramework/Fubuki/Fubuki.h"
#include "Engine.h"
#include "../Basics/BoardIndicator.h"
#include "../Basics/ChessType.h"
#include "BoardManagerBase.generated.h"

UCLASS()
class GOBANG_API ABoardManagerBase : public AActor
{
	GENERATED_BODY()
	
public:	

	const static int BOARD_SIZE = 15;

	// Sets default values for this actor's properties
	ABoardManagerBase();
	
	UFUNCTION(BlueprintCallable, Category = "Game Actions")
		void RemoveGameBoard();

	UFUNCTION(BlueprintCallable, Category = "Game Actions")	
		void InitGameBoard(ABoardLocator* LU, ABoardLocator* RD, bool bBanModeOn);

	/*
	Place a chess on the world board.
		0: Success.
		1: Location taken.
		2: Illegal.
	*/
	UFUNCTION(BlueprintCallable, Category = "Game Actions")
		int PlaceChess(int32 X, int32 Y, EChessType ChessType);

	UFUNCTION(BlueprintCallable, Category = "Game Info")
		bool IsOver() { return board.isOver(); }

	UFUNCTION(BlueprintCallable, Category = "Game Info")
		TArray<FIntPoint> GetReviewData();

	/*
	The winner can only be:
		0: None.
		1: Player 1.
		2: Player 2.
		-1: Board full.
	 */
	UFUNCTION(BlueprintCallable, Category = "Game Info")
		int32 GetWinner();

	//UFUNCTION(BlueprintCallable, Category = "Game Actions")
	std::pair<FIntPoint, FIntPoint> RetractChess();		// This function is not intended to be called in blueprint.
	FIntPoint RetractChessHalf();		// This function is not intended to be called in blueprint.

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
		FIntPoint GetLastPosition();

	/*
	 * Find the direction of winning:
	 * 1: right.
	 * 2: down.
	 * 3: right-down.
	 * 4: left-down.
	 * -1: No winner.
	 */
	UFUNCTION(BlueprintCallable, Category = "Game Info")
		int32 GetWinPositionDir();

	// Get the absoulte location in the world using chess coordinate.
	UFUNCTION(BlueprintCallable, Category = "Game Info")
		FVector GetAbsolutePosition(int32 X, int32 Y, float Z_Offset = 0);

	Board* GetBoardObj() { return &board; }

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
