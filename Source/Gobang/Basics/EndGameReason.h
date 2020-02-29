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
    WHITE_ILLEGAL_ACTION = 2        UMETA(DisplayName = "Black Win, White illegal action"),
    WHITE_SURROUNDER = 3            UMETA(DisplayName = "Black Win, White surrounder"),
    WHITE_TIME_UP = 4               UMETA(DisplayName = "Black Win, White time up"),
    WHITE_ERROR = 5                 UMETA(DisplayName = "Black Win, White unexpected action"),

    WHITE_APPEND_5 = 11             UMETA(DisplayName = "White Win, White append five"),
    BLACK_ILLEGAL_ACTION = 12       UMETA(DisplayName = "White Win, Black illegal action"),
    BLACK_SURROUNDER = 13           UMETA(DisplayName = "White Win, Black surrounder"),
    BLACK_TIME_UP = 14              UMETA(DisplayName = "White Win, Black time up"),
    BLACK_ERROR = 15                UMETA(DisplayName = "White Win, Black unexpected action"),

    DRAW_BOARD_FULL = 21            UMETA(DisplayName = "Draw, board full.")
};
