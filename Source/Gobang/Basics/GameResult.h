// This is a private project, cannot be put at commercial use.

#pragma once

#include "CoreMinimal.h"
#include "GameResult.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EGameResult : uint8
{
    UNKNOWN = 0         UMETA(DisplayName = "Unknown"),

    WIN = 1             UMETA(DisplayName = "Win"),
    LOSE = 2            UMETA(DisplayName = "Lose"),
    DRAW = 3            UMETA(DisplayName = "Draw")
};
