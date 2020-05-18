// This is a private project, cannot be put at commercial use.

#pragma once

#include "CoreMinimal.h"
#include "GameActionType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EGameActionType : uint8
{
    UNKNOWN = 0                     UMETA(DisplayName = "Unknown"),

    ROUND_OVER = 1                  UMETA(DisplayName = "Round over"),
    SURROUNDER = 3                  UMETA(DisplayName = "Surrounder"),
    RETRACT = 4                     UMETA(DisplayName = "Retract"),
    SURROUNDER_ON_CRITICAL = 5      UMETA(DisplayName = "Surrounder On Critical"),

    UNEXPECTED_ACTION = 6           UMETA(DisplayName = "Unexpected Action")

};
