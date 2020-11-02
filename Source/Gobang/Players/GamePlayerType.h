// This is a private project, cannot be put at commercial use.

#pragma once

#include "CoreMinimal.h"
#include "GamePlayerType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EGamePlayerType : uint8
{
    UNKNOWN = 0                 UMETA(DisplayName = "Unknown"),

    LOCAL_PLAYER = 1            UMETA(DisplayName = "Local player"),
    LOCAL_AI = 2                UMETA(DisplayName = "Local AI"),
    REMOTE_PLAYER = 3           UMETA(DisplayName = "Remote player"),
    REMOTE_AI = 4               UMETA(DisplayName = "Remote AI"),
    LOCAL_REMOTE_PLAYER = 5     UMETA(DisplayName = "Local Remote player")
};
