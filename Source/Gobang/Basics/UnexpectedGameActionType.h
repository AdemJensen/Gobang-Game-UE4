// This is a private project, cannot be put at commercial use.

#pragma once

#include "CoreMinimal.h"
#include "UnexpectedGameActionType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EUnexpectedGameActionType : uint8
{
    UNKNOWN = 0             UMETA(DisplayName = "Unknown"),

    DATA_ERROR = 1          UMETA(DisplayName = "Data error or abnormal"),
    CONNECTION_LOST = 2     UMETA(DisplayName = "Connection lost"),
};
