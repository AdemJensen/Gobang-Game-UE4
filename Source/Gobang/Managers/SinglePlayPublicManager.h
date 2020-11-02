// This is a private project, cannot be put at commercial use.

#pragma once

#include "CoreMinimal.h"
#include "PublicManagerBase.h"
#include "../Basics/GameStage.h"
#include "../Basics/ChessType.h"
#include "../Basics/BanMode.h"
#include "SinglePlayPublicManager.generated.h"

/**
 * 
 */
UCLASS()
class GOBANG_API ASinglePlayPublicManager : public APublicManagerBase
{
	GENERATED_BODY()

public:

	virtual EPublicManagerType GetManagerType() { return EPublicManagerType::SINGLE_PLAY; }

	UFUNCTION(BlueprintCallable, Category = "Game Info")
		ASinglePlayPublicManager* InitSinglePlayPublicManagerByValue(int32 DifficultyValue, EBanMode BanModeValue, float HintCoolDownValue, int32 RetractTimesValue, float RoundTimeLimitValue);

	UFUNCTION(BlueprintCallable, Category = "Game Info")
		void SetAiDifficulty(int32 DifficultyLevel) { AiDifficulty = DifficultyLevel; }
	UFUNCTION(BlueprintCallable, Category = "Game Info")
		int32 GetAiDifficulty() { return AiDifficulty; }

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Info")
		int32 AiDifficulty = 0;	// 1~3.
	
};
