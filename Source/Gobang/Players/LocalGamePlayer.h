// This is a private project, cannot be put at commercial use.

#pragma once

#include "CoreMinimal.h"
#include "GamePlayerBase.h"
#include "../Utilities/ThreadWorkerBase.h"
#include "../GobangFramework/KizunaAi/KizunaAi.h"
#include "TimerManager.h"
#include "LocalGamePlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWorkerDoneDelegate, FIntPoint, HintLocation);
/**
 * 
 */
UCLASS()
class GOBANG_API ALocalGamePlayer : public AGamePlayerBase
{
	GENERATED_BODY()

public:

	virtual EGamePlayerType GetPlayerType() { return EGamePlayerType::LOCAL_PLAYER; }

	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "Game Info")
		FWorkerDoneDelegate WorkerDoneDelegate;

	UFUNCTION(BlueprintCallable, Category = "Game Info")
		void StartHintCalculation();

	virtual void OnGameStart();
	virtual void DoRoundOver(FIntPoint ActionLocation);
	
protected:

	KizunaAi MissAi;
	TSharedPtr<FThreadWorkerBase> Worker;
	FTimerHandle AiTimerHandle;
	FTimerDelegate AiTimerDelegate;
	void AiTimerTask();
	bool bAiRunning = false;

};
