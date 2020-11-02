// This is a private project, cannot be put at commercial use.

#pragma once

#include "CoreMinimal.h"
#include "GamePlayerBase.h"
#include "../Utilities/ThreadWorkerBase.h"
#include "../GobangFramework/KizunaAi/KizunaAi.h"
#include "TimerManager.h"
#include "LocalGamePlayer.h"
#include "LocalRemoteGamePlayer.generated.h"

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWorkerDoneDelegate, FIntPoint, HintLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActionDelegate_RoundOver, FIntPoint, ActionLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActionDelegate_EUnexpectedGameAction, EUnexpectedGameActionType, Type);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActionDelegate_General);
/**
 * 
 */
UCLASS()
class GOBANG_API ALocalRemoteGamePlayer : public AGamePlayerBase
{
	GENERATED_BODY()

public:

	virtual EGamePlayerType GetPlayerType() { return EGamePlayerType::LOCAL_REMOTE_PLAYER; }

	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "Game Info")
		FWorkerDoneDelegate WorkerDoneDelegate;
	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "Game Info")
		FActionDelegate_General DoSurrounderDelegate;
	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "Game Info")
		FActionDelegate_General DoRetractDelegate;
	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "Game Info")
		FActionDelegate_RoundOver DoRoundOverDelegate;
	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "Game Info")
		FActionDelegate_EUnexpectedGameAction DoUnexpectedActionDelegate;

	UFUNCTION(BlueprintCallable, Category = "Game Info")
		void StartHintCalculation();

	virtual void OnGameStart();

	virtual void DoRoundOver(FIntPoint ActionLocation) { DoRoundOverDelegate.Broadcast(ActionLocation); UE_LOG(LogTemp, Warning, TEXT("LRGP: Delegate OK.")); }
	virtual void DoSurrounder() { DoSurrounderDelegate.Broadcast(); }
	virtual void DoUnexpectedAction(EUnexpectedGameActionType Type) { DoUnexpectedActionDelegate.Broadcast(Type); }
	virtual void DoRetract() { DoRetractDelegate.Broadcast(); }

	UFUNCTION(BlueprintCallable, Category = "Game Actions")
		virtual void APPLY_DoRoundOver(FIntPoint ActionLocation);
	UFUNCTION(BlueprintCallable, Category = "Game Actions")
		virtual void APPLY_DoSurrounder() { Super::DoSurrounder(); }
	UFUNCTION(BlueprintCallable, Category = "Game Actions")
		virtual void APPLY_DoUnexpectedAction(EUnexpectedGameActionType Type) { Super::DoUnexpectedAction(Type); }
	UFUNCTION(BlueprintCallable, Category = "Game Actions")
		virtual void APPLY_DoRetract() { Super::DoRetract(); }
	
protected:

	KizunaAi MissAi;
	TSharedPtr<FThreadWorkerBase> Worker;
	FTimerHandle AiTimerHandle;
	FTimerDelegate AiTimerDelegate;
	void AiTimerTask();
	bool bAiRunning = false;

};
