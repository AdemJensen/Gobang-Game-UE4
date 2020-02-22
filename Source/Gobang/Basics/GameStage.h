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
    WAIT_FOR_ACTION = 61        UMETA(DisplayName = "Waiting for action"),
    ROUND_OVER = 62             UMETA(DisplayName = "Round over (Action made)"),
    SURROUNDERED = 63           UMETA(DisplayName = "Surroundered"),
    UNEXPECTED_ACTION = 64      UMETA(DisplayName = "Unexpected action occured"),
    RETRACTED = 65              UMETA(DisplayName = "Retracted"),
    TIME_UP = 71                UMETA(DisplayName = "Time up"),
    
    // 101 ~ X post game.
    GAME_OVER = 101             UMETA(DisplayName = "At Result UI")
};
