// This is a private project, cannot be put at commercial use.

#pragma once

#include "CoreMinimal.h"
#include "../Utilities/DowncountHelperBase.h"
#include "GameThreadTicker.generated.h"


/**
 * 
 */
UCLASS()
class GOBANG_API AGameThreadTicker : public ADowncountHelperBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Game Info")
		void StartGameThread() { StartDowncount(TimeLimit, 0.1); }

	UFUNCTION(BlueprintCallable, Category = "Game Info")
		void SetTimeLimit(float TimeSeconds) { TimeLimit = TimeSeconds; }
	UFUNCTION(BlueprintCallable, Category = "Game Info")
		float GetTimeLimit() { return TimeLimit; }

	void OnRemainChanged(float TimeRemain);
	void OnTimeUp();

protected:

	float TimeLimit = 30.0f;
	
};
