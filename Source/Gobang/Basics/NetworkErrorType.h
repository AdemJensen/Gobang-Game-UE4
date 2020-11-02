// This is a private project, cannot be put at commercial use.

#pragma once

#include "CoreMinimal.h"
#include "NetworkErrorType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ENetworkErrorType : uint8
{
    UNKNOWN = 0                     UMETA(DisplayName = "Unknown"),

    // 1~50 Before game
    HANDSHAKE_REQUEST_INVALID = 1   UMETA(DisplayName = "Handshake Request Invalid"),  // Require for server status and information.
    OL_LOGIN_REQUEST = 3            UMETA(DisplayName = "(OL) Login Request"),
    OL_LOGIN_RESULT = 4             UMETA(DisplayName = "(OL) Login Result"),
    OL_REGISTER_REQUEST = 5         UMETA(DisplayName = "(OL) Register Request"),
    OL_REGISTER_RESULT = 6          UMETA(DisplayName = "(OL) Register Result"),
    OL_GET_ROOM_LIST_REQUEST = 7    UMETA(DisplayName = "(OL) Get Room List Request"),
    OL_GET_ROOM_LIST_RESULT = 8     UMETA(DisplayName = "(OL) Get Room List Result"),
    GET_ROOM_INFO_REQUEST = 9       UMETA(DisplayName = "Get Room Info Request"),
    GET_ROOM_INFO_RESULT = 10       UMETA(DisplayName = "Get Room Info Result"),
    OL_ENTER_ROOM_REQUEST = 11      UMETA(DisplayName = "(OL) Enter Room Request"),
    OL_ENTER_ROOM_RESULT = 12       UMETA(DisplayName = "(OL) Enter Room Result"),
    SEND_CHAT_REQUEST = 13          UMETA(DisplayName = "Send Chat Request"),
    SEND_CHAT_RESULT = 14           UMETA(DisplayName = "Send Chat Result"),
    INCOME_CHAT_MSG = 15            UMETA(DisplayName = "Income Chat Message"),
    INCOME_CHAT_REPLY = 16          UMETA(DisplayName = "Income Chat Reply"),
    GET_READY_REQUEST = 17          UMETA(DisplayName = "Get Ready Request"),
    GET_READY_RESULT = 18           UMETA(DisplayName = "Get Ready Result"),
    CANCEL_READY_REQUEST = 19       UMETA(DisplayName = "Cancel Ready Request"),
    CANCEL_READY_RESULT = 20        UMETA(DisplayName = "Cancel Ready Result"),
    LN_ENTER_ROOM_REQUEST = 21      UMETA(DisplayName = "(LAN) Enter Room Request"),
    LN_ENTER_ROOM_RESULT = 22       UMETA(DisplayName = "(LAN) Enter Room Result"),
    PLAYER_CHANGE_MSG = 23          UMETA(DisplayName = "Player Change Message"),  // Player change color, or other player quit
    PLAYER_CHANGE_REPLY = 24        UMETA(DisplayName = "Player Change Reply"),

    GAME_START_MSG = 49             UMETA(DisplayName = "Game Start Message"),
    GAME_START_REPLY = 50           UMETA(DisplayName = "Game Start Reply"),

    // 51~100 Ingame
    SERVER_PING_MSG = 51            UMETA(DisplayName = "Server Ping Message"),     // Include the timer info, called every 0.1 second.
    SERVER_PING_REPLY = 52          UMETA(DisplayName = "Server Ping Reply"),
    CLIENT_ACTION_REQUEST = 52      UMETA(DisplayName = "Client Action Request"),
    CLIENT_ACTION_RESULT = 53       UMETA(DisplayName = "Client Action Result"),
    SERVER_WARNING_MSG = 61         UMETA(DisplayName = "Server warning Message"),
    SERVER_WARNING_REPLY = 61       UMETA(DisplayName = "Server warning Reply"),    // Might not useful.
    SERVER_ERROR_MSG = 71           UMETA(DisplayName = "Server error Message"),    
    SERVER_ERROR_REPLY = 71         UMETA(DisplayName = "Server error Reply"),      // Might not useful.

    // 101~150 Aft Game
    OL_GAME_END_MSG = 101           UMETA(DisplayName = "(OL) Game End Message"),

};
