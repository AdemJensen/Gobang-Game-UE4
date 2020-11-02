// This is a private project, cannot be put at commercial use.

#pragma once

#include "CoreMinimal.h"
#include "BanMode.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EBanMode : uint8
{
    UNKNOWN = 0             UMETA(DisplayName = "Unknown"),

    ON_ILLEGAL_BANNED = 1   UMETA(DisplayName = "On, illegal position banned"),
    ON_LOSE_IF_ILLEGAL = 2  UMETA(DisplayName = "On, no hints (Lose if action illegal)"),
    OFF = 3                 UMETA(DisplayName = "Off")
};
