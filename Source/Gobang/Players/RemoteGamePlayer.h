// This is a private project, cannot be put at commercial use.

#pragma once

#include "CoreMinimal.h"
#include "GamePlayerBase.h"
#include "../Utilities/ThreadWorkerBase.h"
#include "../GobangFramework/Fubuki/Fubuki.h"
#include "../GobangFramework/KizunaAi/KizunaAi.h"
#include "TimerManager.h"
#include "RemoteGamePlayer.generated.h"

/**
 * 
 */
UCLASS()
class GOBANG_API ARemoteGamePlayer : public AGamePlayerBase
{
	GENERATED_BODY()
	
public:

	virtual EGamePlayerType GetPlayerType() { return EGamePlayerType::REMOTE_PLAYER; }
	
	/*virtual void OnGameStart();
	virtual void OnRoundStart();
	virtual void OnInterrupt();
	virtual void OnRetract(FIntPoint RetractPosition1, FIntPoint RetractPosition2);*/

};
