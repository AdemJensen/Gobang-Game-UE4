// This is a private project, cannot be put at commercial use.

#pragma once

#include "CoreMinimal.h"
#include "../Utilities/DowncountHelperBase.h"
#include "../Basics/EndGameReason.h"
#include "GameThreadTicker.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRoundOverAction, AGamePlayerBase*, TargetPlayer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameEndAction, EEndGameReason, EndGameReason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRefreshUiAction, AGamePlayerBase*, TargetPlayer, float, TimeRemain);
/**
 * 
 */
UCLASS()
class GOBANG_API AGameThreadTicker : public ADowncountHelperBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Actions")
		FRoundOverAction RoundOverAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Actions")
		FRoundOverAction TimeUpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Actions")
		FGameEndAction GameEndAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Actions")
		FRefreshUiAction RefreshUiAction;

	UFUNCTION(BlueprintCallable, Category = "Game Info")
		void StartGameThread() { StartDowncount(TimeLimit, 0.1); }

	UFUNCTION(BlueprintCallable, Category = "Game Info")
		void SetTimeLimit(float TimeSeconds) { TimeLimit = TimeSeconds; }
	UFUNCTION(BlueprintCallable, Category = "Game Info")
		float GetTimeLimit() { return TimeLimit; }

	void OnRoundOver(); // Called inside for Thread.

	void OnRemainChanged(float TimeRemain);
	void OnTimeUp();

	

protected:

	float TimeLimit = 30.0f;
	
};
