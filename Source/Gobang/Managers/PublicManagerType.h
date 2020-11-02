// This is a private project, cannot be put at commercial use.

#pragma once

#include "CoreMinimal.h"
#include "PublicManagerType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EPublicManagerType : uint8
{
    UNKNOWN = 0         UMETA(DisplayName = "Unknown"),

    SINGLE_PLAY = 1     UMETA(DisplayName = "SinglePlay"),
    MULTI_PLAY = 2      UMETA(DisplayName = "Multiplay")
};
