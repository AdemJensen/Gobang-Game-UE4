// This is a private project, cannot be put at commercial use.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoardManagerBase.h"
#include "PublicManagerBase.h"
#include "../Basics/ChessType.h"
#include "../Basics/ProgramStage.h"
#include "GameManagerBase.generated.h"

UCLASS()
class GOBANG_API AGameManagerBase : public AActor
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this actor's properties
	AGameManagerBase();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Game Info")
		ABoardManagerBase* BoardManager;	// Only valid when at game.

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Game Info")
		APublicManagerBase* PublicManager;	// Only valid when at game.

	UFUNCTION(BlueprintCallable, Category = "Game Info")
		void SetProgramStage(EProgramStage ProgramStage) { Stage = ProgramStage; }
	UFUNCTION(BlueprintCallable, Category = "Game Info")
		EProgramStage GetProgramStage() { return Stage; }

	/*
	Prepare the parameters for gameplay.
	Return:
		- 0: AOK, No problem.
		- 1: The world object is invalid.
		- 2: The Board Manager is invalid.
		- 3: The Public Manager is invalid.
	*/
	UFUNCTION(BlueprintCallable, Category = "Game Actions")
		int OnGameStart();

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

protected:

	UPROPERTY(EditAnywhere, Category = "Game Info")
		EChessType PlayerChess = EChessType::UNKNOWN;

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

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
