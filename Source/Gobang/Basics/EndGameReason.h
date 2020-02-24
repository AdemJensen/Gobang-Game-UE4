// This is a private project, cannot be put at commercial use.

#pragma once

#include "CoreMinimal.h"
#include "EndGameReason.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EEndGameReason : uint8
{
    UNKNOWN = 0                     UMETA(DisplayName = "Unknown"),

    BLACK_APPEND_5 = 1              UMETA(DisplayName = "Black Win, Black append five"),
    WHITE_ILEGAL_ACTION = 2         UMETA(DisplayName = "Black Win, White ilegal action"),
    WHITE_RIVAL_SURROUNDER = 3      UMETA(DisplayName = "Black Win, White surrounder"),
    WHITE_RIVAL_ERROR = 4           UMETA(DisplayName = "Black Win, White unexpected action"),

    WHITE_APPEND_5 = 11             UMETA(DisplayName = "White Win, White append five"),
    BLACK_YOU_ILEGAL_ACTION = 12    UMETA(DisplayName = "White Win, Black ilegal action"),
    BLACK_YOU_SURROUNDER = 13       UMETA(DisplayName = "White Win, Black surrounder"),
    BLACK_YOU_ERROR = 14            UMETA(DisplayName = "White Win, Black unexpected action"),

    DRAW_BOARD_FULL = 21            UMETA(DisplayName = "Draw, board full.")
};
