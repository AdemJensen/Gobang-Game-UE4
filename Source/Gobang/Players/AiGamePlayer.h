// This is a private project, cannot be put at commercial use.

#pragma once

#include "CoreMinimal.h"
#include "GamePlayerBase.h"
#include "../Utilities/ThreadWorkerBase.h"
#include "../GobangFramework/Fubuki/Fubuki.h"
#include "../GobangFramework/KizunaAi/KizunaAi.h"
#include "TimerManager.h"
#include "AiGamePlayer.generated.h"

/**
 * 
 */
UCLASS()
class GOBANG_API AAiGamePlayer : public AGamePlayerBase
{
	GENERATED_BODY()
	
public:

	virtual EGamePlayerType GetPlayerType() { return EGamePlayerType::LOCAL_AI; }
	
	virtual void OnGameStart();
	virtual void OnRoundStart();
	virtual void OnInterrupt();
	virtual void OnRetract(FIntPoint RetractPosition1, FIntPoint RetractPosition2);

	UFUNCTION(BlueprintCallable, Category = "Game Info")
		void SetAiLevel(int32 Level) { AiLevel = Level; }
	UFUNCTION(BlueprintCallable, Category = "Game Info")
		int32 GetAiLevel() { return AiLevel; }

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Info")
		int32 AiLevel = 2;

	TSharedPtr<FThreadWorkerBase> Worker;

	KizunaAi MissAi;
	Fubuki MissFu;

	FTimerHandle AiTimerHandle;
	FTimerDelegate AiTimerDelegate;
	void AiTimerTask();

};
