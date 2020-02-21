// This is a private project, cannot be put at commercial use.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Basics/GameStage.h"
#include "../Basics/ChessType.h"
#include "../Basics/BanMode.h"
#include "PublicManagerBase.generated.h"

UCLASS()
class GOBANG_API APublicManagerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APublicManagerBase();

	UFUNCTION(BlueprintCallable, Category = "Game Status")
		void SetGameStage(EGameStage GameStage) { Stage = GameStage; }
	UFUNCTION(BlueprintCallable, Category = "Game Status")
		EGameStage GetGameStage() { return Stage; }

	UFUNCTION(BlueprintCallable, Category = "Game Info")
		void SetSelfChess(EChessType ChessType) { SelfChess = ChessType; }
	UFUNCTION(BlueprintCallable, Category = "Game Info")
		EChessType GetSelfChess() { return SelfChess; }

	UFUNCTION(BlueprintCallable, Category = "Game Info")
		void SetBanMode(EBanMode BoardBanMode) { BanMode = BoardBanMode; }
	UFUNCTION(BlueprintCallable, Category = "Game Info")
		EBanMode GetBanMode() { return BanMode; }

	UFUNCTION(BlueprintCallable, Category = "Game Info")
		void SetTotalRetractTimes(int32 TotalRetractTimes) { R_RetractTimes = T_RetractTimes = TotalRetractTimes; }
	UFUNCTION(BlueprintCallable, Category = "Game Info")
		int32 GetTotalRetractTimes() { return T_RetractTimes; }
	UFUNCTION(BlueprintCallable, Category = "Game Info")
		int32 GetRemainRetractTimes() { return R_RetractTimes; }

	UFUNCTION(BlueprintCallable, Category = "Game Info")
		void SetRoundTimeLimit(float TimeLimit) { RoundTimeLimit = TimeLimit; }
	UFUNCTION(BlueprintCallable, Category = "Game Info")
		float GetRoundTimeLimit() { return RoundTimeLimit; }

	UFUNCTION(BlueprintCallable, Category = "Game Info")
		void SetHintCoolDownTime(float CoolDown) { HintCoolDownTime = CoolDown; }
	UFUNCTION(BlueprintCallable, Category = "Game Info")
		float GetHintCoolDownTime() { return HintCoolDownTime; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Game Status")
		EGameStage Stage = EGameStage::UNKNOWN;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Info")
		EChessType SelfChess = EChessType::UNKNOWN;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Info")
		EBanMode BanMode = EBanMode::UNKNOWN;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Info")
		int32 T_RetractTimes = 0;	// 0 if off, -1 if no limit.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Info")
		int32 R_RetractTimes = 0;	// RetractTimes Remain

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Info")
		float RoundTimeLimit = 0;	// 0 if no limit.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Info")
		float HintCoolDownTime = 0;	// 0 if no cool down, -1 if off.

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
