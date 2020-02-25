// This is a private project, cannot be put at commercial use.

#pragma once

#include "CoreMinimal.h"
#include "GamePlayerBase.h"
#include "LocalGamePlayer.generated.h"

/**
 * 
 */
UCLASS()
class GOBANG_API ALocalGamePlayer : public AGamePlayerBase
{
	GENERATED_BODY()

public:

	virtual EGamePlayerType GetPlayerType() { return EGamePlayerType::LOCAL_PLAYER; }
	
};
