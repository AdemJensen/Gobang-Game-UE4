// This is a private project, cannot be put at commercial use.

#pragma once

#include "CoreMinimal.h"
#include "GamePlayerBase.h"
#include "AiGamePlayer.generated.h"

/**
 * 
 */
UCLASS()
class GOBANG_API AAiGamePlayer : public AGamePlayerBase
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "Game Info")
		void SetAiLevel(int32 Level) { AiLevel = Level; }
	UFUNCTION(BlueprintCallable, Category = "Game Info")
		int32 GetAiLevel() { return AiLevel; }

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Info")
		int32 AiLevel = 2;

};
