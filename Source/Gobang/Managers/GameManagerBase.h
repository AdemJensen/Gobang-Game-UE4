// This is a private project, cannot be put at commercial use.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoardManagerBase.h"
#include "PublicManagerBase.h"
#include "../Basics/ChessType.h"
#include "../Basics/ProgramStage.h"
#include "../Players/GamePlayerBase.h"
#include "SinglePlayPublicManager.h"
#include "GameThreadTicker.h"
#include "IndicationManager.h"
#include "../Basics/UnexpectedGameActionType.h"
#include "GameManagerBase.generated.h"

UCLASS()
class GOBANG_API AGameManagerBase : public AActor
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this actor's properties
	AGameManagerBase();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Info")
		ABoardManagerBase* BoardManager;	// Only valid when at game.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Info")
		APublicManagerBase* PublicManager;	// Only valid when at game.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Info")
		AIndicationManager* IndicationManager;	// Valid all the time.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Info")
		AGameThreadTicker* GameThread;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Info")
		ASinglePlayPublicManager* LastSinglePlayPublicManager;	// Valid in the first place.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Info")
		APublicManagerBase* LastMultiPlayPublicManager;	// Valid in the first place.

	UFUNCTION(BlueprintCallable, Category = "Game Info")
		void SetProgramStage(EProgramStage ProgramStage) { Stage = ProgramStage; }
	UFUNCTION(BlueprintCallable, Category = "Game Info")
		EProgramStage GetProgramStage() { return Stage; }

	/*
	Prepare the parameters for gameplay.
	*/
	UFUNCTION(BlueprintCallable, Category = "Game Actions")
		void DoGameStart();

	UFUNCTION(BlueprintCallable, Category = "Game Info")
		void SetLastPlayChessType(EChessType LastType) { LastPlayChessType = LastType; }
	UFUNCTION(BlueprintCallable, Category = "Game Info")
		EChessType GetLastPlayChessType() { return LastPlayChessType; }

	UFUNCTION(BlueprintCallable, Category = "Game Actions")
		void DoRoundOver(FIntPoint ActionLocation);
	UFUNCTION(BlueprintCallable, Category = "Game Actions")
		void DoSurrounder(EChessType SurrounderTarget);
	UFUNCTION(BlueprintCallable, Category = "Game Actions")
		void DoUnexpectedAction(EUnexpectedGameActionType Type);
	UFUNCTION(BlueprintCallable, Category = "Game Actions")
		void DoRetract();

	UFUNCTION(BlueprintCallable, Category = "Game Info")
		AGameThreadTicker* GetGameThreadObj() { return GameThread; }

	UFUNCTION(BlueprintCallable, Category = "Game Info")
		AGamePlayerBase* GetGamePlayer(EChessType PlayerType) { return PlayerType == EChessType::BLACK ? GamePlayer_Black : GamePlayer_White; }
	UFUNCTION(BlueprintCallable, Category = "Game Info")
		void SetGamePlayer(EChessType PlayerType, AGamePlayerBase* GamePlayer);
	

protected:

	UPROPERTY(EditAnywhere, Category = "Game Info")
		EProgramStage Stage = EProgramStage::UNKNOWN;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Info")
		EChessType LastPlayChessType = EChessType::BLACK;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Info")
		AGamePlayerBase* GamePlayer_Black;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Info")
		AGamePlayerBase* GamePlayer_White;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
