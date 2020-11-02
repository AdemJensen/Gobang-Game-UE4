// Copyright 2017-2020 David Romanski (Socke). All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DNSClientSocketServer.h"
#include "EventBean.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/Sockets/Public/Sockets.h"
#include "Runtime/Networking/Public/Networking.h"
#include "Runtime/Sockets/Public/SocketSubsystem.h"
#include "Runtime/Core/Public/Misc/DateTime.h"
#include "Runtime/Core/Public/HAL/PlatformFilemanager.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Runtime/Core/Public/HAL/FileManager.h"
#include "Runtime/Core/Public/Misc/Base64.h"
#include "Runtime/Core/Public/Misc/SecureHash.h"
#include "Modules/ModuleManager.h"
#include "SocketServer.generated.h"

class FTCPClientSendDataToServerThread;
class FTCPClientReceiveDataFromServerThread;

UENUM(BlueprintType)
enum class EFileFunctionsSocketServerDirectoryType : uint8
{
	E_gd	UMETA(DisplayName = "Game directory"),
	E_ad 	UMETA(DisplayName = "Absolute directory")
};

UENUM(BlueprintType)
enum class EHTTPSocketServerFileDownloadResumeType : uint8
{
	E_OVERWRITE		UMETA(DisplayName = "Overwrite"),
	E_NOT_OVERWRITE UMETA(DisplayName = "Cancel Download"),
	E_RESUME		UMETA(DisplayName = "Resume")
};

UENUM(BlueprintType)
enum class EServerSocketConnectionEventType : uint8
{
	E_Server 	UMETA(DisplayName = "Server"),
	E_Client	UMETA(DisplayName = "Client")

};

UENUM(BlueprintType)
enum class EServerSocketConnectionProtocol : uint8
{
	E_NotSet 	UMETA(DisplayName = "NotSet"),
	E_TCP 		UMETA(DisplayName = "TCP"),
	E_UDP		UMETA(DisplayName = "UDP")

};

UENUM(BlueprintType)
enum class EServerSocketConnectionCheckPortType : uint8
{
	E_TCP 		UMETA(DisplayName = "TCP"),
	E_UDP		UMETA(DisplayName = "UDP")

};

UENUM(BlueprintType)
enum class EReceiveFilterServer : uint8
{
	E_SAB 	UMETA(DisplayName = "Message And Bytes"),
	E_S		UMETA(DisplayName = "Message"),
	E_B		UMETA(DisplayName = "Bytes")

};

UENUM(BlueprintType)
enum class ESocketPlatformServer : uint8
{
	E_SSS_SYSTEM		UMETA(DisplayName = "System"),
	E_SSS_DEFAULT 		UMETA(DisplayName = "Auto"),
	E_SSS_WINDOWS		UMETA(DisplayName = "WINDOWS"),
	E_SSS_MAC			UMETA(DisplayName = "MAC"),
	E_SSS_IOS			UMETA(DisplayName = "IOS"),
	E_SSS_UNIX			UMETA(DisplayName = "UNIX"),
	E_SSS_ANDROID		UMETA(DisplayName = "ANDROID"),
	E_SSS_PS4			UMETA(DisplayName = "PS4"),
	E_SSS_XBOXONE		UMETA(DisplayName = "XBOXONE"),
	E_SSS_HTML5			UMETA(DisplayName = "HTML5"),
	E_SSS_SWITCH		UMETA(DisplayName = "SWITCH")

};

UENUM(BlueprintType)
enum class ESocketServerUDPSocketType : uint8
{
	E_SSS_SERVER 	UMETA(DisplayName = "Use Server Socket"),
	E_SSS_CLIENT	UMETA(DisplayName = "Use Client Socket")

};

struct IpAndPortStruct {
	bool			success;
	FString			errorMessage;
	TArray<int32>	ipArray;
	int32			port;
};

USTRUCT(BlueprintType)
struct FClientSocketSession
{
	GENERATED_USTRUCT_BODY()

	FString ip;
	int32	port;
	FString sessionID;
	FString serverID;
	FSocket* socket = nullptr;

	FTCPClientSendDataToServerThread* sendThread = nullptr;
	FTCPClientReceiveDataFromServerThread* recieverThread = nullptr;

	EServerSocketConnectionProtocol protocol;
};


USTRUCT(BlueprintType)
struct FSocketServerDownloadFileInfo
{
	GENERATED_USTRUCT_BODY()

	float size;
	float megaBytesReceived;
	float megaBytesLeft;
	float percentDownload;
	float megaBit;
	FString fileName;
	FString serverID;
};

USTRUCT(BlueprintType)
struct FSocketServerUploadFileInfo
{
	GENERATED_USTRUCT_BODY()

	float size;
	float megaBytesSend;
	float megaBytesLeft;
	float percentUpload;
	float megaBit;
	FString fileName;
	FString serverID;
};

#ifndef __FileFunctionsSocketServer
#define __FileFunctionsSocketServer
#include "FileFunctionsSocketServer.h"
#endif

#ifndef __SocketServerBPLibrary
#define __SocketServerBPLibrary
#include "SocketServerBPLibrary.h"
#endif

#ifndef __SocketServerPluginUDPServer
#define __SocketServerPluginUDPServer
#include "SocketServerPluginUDPServer.h"
#endif

#ifndef __SocketServerPluginTCPServer
#define __SocketServerPluginTCPServer
#include "SocketServerPluginTCPServer.h"
#endif

class FSocketServerModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};