// This is a private project, cannot be put at commercial use.

#pragma once

#include "CoreMinimal.h"
#include "ProgramStage.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EProgramStage : uint8
{
    UNKNOWN = 0                         UMETA(DisplayName = "Unknown"),
    // 1 ~ 50 = before game.
    AT_MAIN_UI = 1                      UMETA(DisplayName = "At Main UI"),
    AT_SELECT_MODE_UI = 2               UMETA(DisplayName = "At Select Mode UI"),

    AT_SINGLE_GAME_SETTINGS_UI = 3      UMETA(DisplayName = "At Single Game Settings UI"),
    AT_SINGLE_CHESS_SELECTION_UI = 5    UMETA(DisplayName = "At Single Chess Selection UI"),

    AT_MULTI_TITLE_UI = 11              UMETA(DisplayName = "At Multi Title UI"),

    AT_PUBLIC_NET_TITLE_UI = 31         UMETA(DisplayName = "At Public Network Title UI"),

    AT_LOCAL_NET_TITLE_UI = 36          UMETA(DisplayName = "At Local Network Title UI"),
    AT_LOCAL_NET_CREATE_UI = 37         UMETA(DisplayName = "At Local Network Create UI"),
    AT_LOCAL_NET_JOIN_UI = 38           UMETA(DisplayName = "At Local Network Join UI"),
    AT_LOCAL_NET_ROOM_UI = 39           UMETA(DisplayName = "At Local Network Room UI"),
    AT_LOCAL_NET_ROOM_SETTINGS_UI = 40  UMETA(DisplayName = "At Local Network Room Settings UI"),

    // 51 ~ 100 ingame process.
    IN_GAME = 51                        UMETA(DisplayName = "In Game"),
    
    // 101 ~ X post game.
    AT_RESULT_UI = 101                  UMETA(DisplayName = "At Result UI")
};
