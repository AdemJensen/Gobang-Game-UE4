// This is a private project, cannot be put at commercial use.

#pragma once

#include "CoreMinimal.h"
#include "ChessType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EChessType : uint8
{
    UNKNOWN = 0         UMETA(DisplayName = "Unknown"),

    BLACK = 1           UMETA(DisplayName = "Black Chess"),
    WHITE = 2           UMETA(DisplayName = "White Chess")
};
