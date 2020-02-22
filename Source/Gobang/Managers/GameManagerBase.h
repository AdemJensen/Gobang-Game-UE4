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
#include "../Basics/UnexpectedGameActionType.h"
#include "GameManagerBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeUiAction, AGamePlayerBase*, TargetPlayer);
UCLASS()
class GOBANG_API AGameManagerBase : public AActor
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this actor's properties
	AGameManagerBase();

	FChangeUiAction ChangeUiAction;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Info")
		ABoardManagerBase* BoardManager;	// Only valid when at game.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Info")
		APublicManagerBase* PublicManager;	// Only valid when at game.

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

	// Indicators
	UFUNCTION(BlueprintCallable, Category = "Game Actions")
		void SetPlayerIndicator(int32 X, int32 Y, bool IsOk);
	UFUNCTION(BlueprintCallable, Category = "Game Actions")
		void SetHintIndicator(int32 X, int32 Y);
	UFUNCTION(BlueprintCallable, Category = "Game Actions")
		void SetLastIndicator(int32 X, int32 Y);
	UFUNCTION(BlueprintCallable, Category = "Game Actions")
		void HideHintIndicator();
	UFUNCTION(BlueprintCallable, Category = "Game Actions")
		void HidePlayerIndicator();
	UFUNCTION(BlueprintCallable, Category = "Game Actions")
		void HideLastIndicator();

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Presentations")
		ABoardIndicator* Indicator_Player;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Presentations")
		ABoardIndicator* Indicator_Last;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Presentations")
		ABoardIndicator* Indicator_Hint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Presentations")
		USoundWave* PlaceChessAudio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Info")
		EChessType LastPlayChessType = EChessType::BLACK;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Info")
		AGamePlayerBase* GamePlayer_Black;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Info")
		AGamePlayerBase* GamePlayer_White;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	AGameThreadTicker* GameThread;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
