// This is a private project, cannot be put at commercial use.

#pragma once

#include "CoreMinimal.h"
#include "../Utilities/DowncountHelperBase.h"
#include "../Basics/EndGameReason.h"
#include "GameThreadTicker.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRoundOverAction, AGamePlayerBase*, TargetPlayer);	// Target is next player.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRetractAction, AGamePlayerBase*, TargetPlayer);
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

	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Game Actions")
		FRoundOverAction RoundOverAction;

	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Game Actions")
		FRetractAction RetractAction;

	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Game Actions")
		FGameEndAction CriticalAction;

	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Game Actions")
		FGameEndAction GameEndAction;

	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Game Actions")
		FRefreshUiAction RefreshUiAction;

	UFUNCTION(BlueprintCallable, Category = "Game Info")
		void StartGameThread() { StartDowncount(TimeLimit, 0.1); }

	UFUNCTION(BlueprintCallable, Category = "Game Info")
		void SetTimeLimit(float TimeSeconds) { TimeLimit = TimeSeconds; }
	UFUNCTION(BlueprintCallable, Category = "Game Info")
		float GetTimeLimit() { return TimeLimit; }

	UFUNCTION(BlueprintCallable, Category = "Game Info")
		void SetActionPoint(FIntPoint Point) { ActionPoint = Point; }
	UFUNCTION(BlueprintCallable, Category = "Game Info")
		FIntPoint GetActionPoint() { return ActionPoint; }

	void OnCritical(EEndGameReason Reason); // Called inside for Thread.
	void OnRoundOver(); // Called inside for Thread.

	void OnRemainChanged(float TimeRemain);
	void OnTimeUp();

	void Interrupt();

protected:

	float TimeLimit = 30.0f;

	FIntPoint ActionPoint;
	
};
