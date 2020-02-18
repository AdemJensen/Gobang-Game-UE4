// This is a private project, cannot be put at commercial use.

#pragma once

#include "CoreMinimal.h"
#include "GameStage.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EGameStage : uint8
{
    UNKNOWN = 0                 UMETA(DisplayName = "Unknown"),
    // 1 ~ 50 = before actual game. (Preserved)

    // 51 ~ 100 ingame process.
    WAIT_FOR_BLACK = 61          UMETA(DisplayName = "In Game Black"),
    WAIT_FOR_WHITE = 81          UMETA(DisplayName = "In Game White"),
    
    // 101 ~ X post game.
    GAME_OVER = 101           UMETA(DisplayName = "At Result UI")
};
