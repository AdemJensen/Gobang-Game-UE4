// This is a private project, cannot be put at commercial use.

#pragma once

#include "CoreMinimal.h"
#include "../Utilities/DowncountHelperBase.h"
#include "../Basics/EndGameReason.h"
#include "CriticalRetractDownCounter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCriticalAction, EEndGameReason, EndGameReason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRefreshCriticalUiAction, float, TimeRemain);
/**
 * 
 */
UCLASS()
class GOBANG_API ACriticalRetractDownCounter : public ADowncountHelperBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Game Actions")
		FCriticalAction GameEndAction;

	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Game Actions")
		FRefreshCriticalUiAction RefreshUiAction;

	UFUNCTION(BlueprintCallable, Category = "Game Info")
		void StartFinalDowncount();

	UFUNCTION(BlueprintCallable, Category = "Game Info")
		void SetFutureReason(EEndGameReason FutureReason) { EndGameReason = FutureReason; }
	UFUNCTION(BlueprintCallable, Category = "Game Info")
		EEndGameReason GetFutureReason() { return EndGameReason; }

	void OnRemainChanged(float TimeRemain);
	void OnTimeUp();

protected:

	EEndGameReason EndGameReason;
	
};
