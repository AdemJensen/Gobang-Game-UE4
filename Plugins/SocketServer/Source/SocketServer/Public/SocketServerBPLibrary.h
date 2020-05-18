// Copyright 2017-2020 David Romanski (Socke). All Rights Reserved.

#pragma once

#include "SocketServer.h"
#include "SocketServerBPLibrary.generated.h"

class USocketServerPluginUDPServer;
class USocketServerPluginTCPServer;

UCLASS()
class SOCKETSERVER_API USocketServerBPLibrary : public UObject
{
	GENERATED_UCLASS_BODY()


public:

	static USocketServerBPLibrary *socketServerBPLibrary;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SocketServer")
		static USocketServerBPLibrary* getSocketServerTarget();

	//Delegates
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FsocketServerConnectionEventDelegate, EServerSocketConnectionEventType, type, bool, success, FString, message, FString, sessionID, FString, serverID);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FserverReceiveTCPMessageEventDelegate, FString, sessionID, FString, message, const TArray<uint8>&, byteArray, FString, serverID);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FserverReceiveTCPFileDownloadEventDelegate, int32, statusCode, FString, statusMessage, FString, sessionID, FString, serverID, FString, fullPath);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FserverReceiveTCPFileDownloadProgressEventDelegate, FSocketServerDownloadFileInfo, fileDownloadInfo);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FserverSendTCPFileUploadEventDelegate, int32, statusCode, FString, statusMessage, FString, sessionID, FString, serverID, FString, fullPath);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FserverSendTCPFileUploadProgressEventDelegate, FSocketServerUploadFileInfo, fileUploadInfo);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FsocketServerUDPConnectionEventDelegate, bool, success, FString, message, FString, serverID);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FserverReceiveUDPMessageEventDelegate, FString, sessionID, FString, message, const TArray<uint8>&, byteArray,FString, serverID);

	
	UFUNCTION()
		void socketServerConnectionEventDelegate(const EServerSocketConnectionEventType type, const bool success, const FString message, const FString sessionID, const FString serverID);
	UPROPERTY(BlueprintAssignable, Category = "SocketServer|TCP|Events|ConnectionInfo")
		FsocketServerConnectionEventDelegate onsocketServerConnectionEventDelegate;
	UFUNCTION()
		void serverReceiveTCPMessageEventDelegate(const FString sessionID, const FString message, const TArray<uint8>& byteArray, const FString serverID);
	UPROPERTY(BlueprintAssignable, Category = "SocketServer|TCP|Events|ReceiveMessage")
		FserverReceiveTCPMessageEventDelegate onserverReceiveTCPMessageEventDelegate;
	//UFUNCTION()
		void serverReceiveTCPFileDownloadEventDelegate(const int32 statusCode, const FString statusMessage,const FString sessionID, const FString serverID, const FString fullPath);
	//UPROPERTY(BlueprintAssignable, Category = "SocketServer|TCP|Events|FileDownload")
		FserverReceiveTCPFileDownloadEventDelegate onserverReceiveTCPFileDownloadEventDelegate;
	//UFUNCTION()
		void serverReceiveTCPFileDownloadProgressEventDelegate(const FSocketServerDownloadFileInfo fileInfo);
	//UPROPERTY(BlueprintAssignable, Category = "SocketServer|TCP|Events|FileDownloadProgress")
		FserverReceiveTCPFileDownloadProgressEventDelegate onserverReceiveTCPFileDownloadProgressEventDelegate;
	//UFUNCTION()
		void serverSendTCPFileUploadEventDelegate(const int32 statusCode, const FString statusMessage, const FString sessionID, const FString serverID, const FString fullPath);
	//UPROPERTY(BlueprintAssignable, Category = "SocketServer|TCP|Events|FileUpload")
		FserverSendTCPFileUploadEventDelegate onserverSendTCPFileUploadEventDelegate;
	//UFUNCTION()
		void serverSendTCPFileUploadProgressEventDelegate(const FSocketServerUploadFileInfo fileInfo);
	//UPROPERTY(BlueprintAssignable, Category = "SocketServer|TCP|Events|FileUploadProgress")
		FserverSendTCPFileUploadProgressEventDelegate onserverSendTCPFileUploadProgressEventDelegate;
	UFUNCTION()
		void socketServerUDPConnectionEventDelegate(const bool success, const FString message, const FString serverID);
	UPROPERTY(BlueprintAssignable, Category = "SocketServer|UDP|Events|ConnectionInfo")
		FsocketServerUDPConnectionEventDelegate onsocketServerUDPConnectionEventDelegate;
	UFUNCTION()
		void serverReceiveUDPMessageEventDelegate(const FString sessionID, const FString message, const TArray<uint8>& byteArray, const FString serverID);
	UPROPERTY(BlueprintAssignable, Category = "SocketServer|UDP|Events|ReceiveMessage")
		FserverReceiveUDPMessageEventDelegate onserverReceiveUDPMessageEventDelegate;


	UFUNCTION(BlueprintCallable, Category = "SocketServer")
		static void breakDownloadFileInfo(FSocketServerDownloadFileInfo fileDownloadInfo, float& size, float& megaBytesLeft, float& megaBytesReceived, float& percentDownload, float& megaBit, FString& fileName,	FString& serverID);
	UFUNCTION(BlueprintCallable, Category = "SocketServer")
		static void breakUploadFileInfo(FSocketServerUploadFileInfo fileUploadInfo, float& size, float& megaBytesLeft, float& megaBytesSend, float& percentUpload, float& megaBit, FString& fileName, FString& serverID);

	/**
	*Get all Session IDs
	*@param optionalServerID With one server the field can remain empty. If there are several servers, the ServerID should be entered here or the newest server is automatically taken.
	*/
	UFUNCTION(BlueprintCallable, Category = "SocketServer")
		void serverPluginGetSocketSessionIds(const FString optionalServerID, TArray<FString>& sessionIDs);
	UFUNCTION(BlueprintCallable, Category = "SocketServer")
		void serverPluginGetSocketSessionInfo(const FString sessionID, bool &sessionFound, FString &IP, int32 &port, EServerSocketConnectionProtocol &connectionProtocol, FString& serverID);
	UFUNCTION(BlueprintCallable, Category = "SocketServer")
		void serverPluginGetSocketSessionInfoByServerID(const FString serverID, const FString sessionID, bool& sessionFound, FString& IP, int32& port, EServerSocketConnectionProtocol& connectionProtocol);
	/**
	*Close a connection and remove the session
	*@param optionalServerID With one server the field can remain empty. If there are several servers, the ServerID should be entered here or the newest server is automatically taken.
	*/
	UFUNCTION(BlueprintCallable, Category = "SocketServer")
		void removeSessionAndCloseConnection(FString sessionId, FString optionalServerID);

	struct IpAndPortStruct checkIpAndPort(FString IP, int32 port);


	//UDP
	/**
	*Start UDP Server
	*@param domainOrIP IP or Domain to listen
	*@param port port to listen
	*@param multicast This allows several servers to be started on different computers in the LAN with the same IP.
	*@param receiveFilter This allows you to decide which data type you want to receive. If you receive files it makes no sense to convert them into a string.
	*@param customServerID Optionally you can assign your own ServerID like "myAuthentificationServer" or "fileServer"
	*@param maxPacketSize sets the maximum UDP packet size. More than 65507 is not possible.
	*/
	UFUNCTION(BlueprintCallable, Category = "SocketServer|UDP")
		void startUDPServer(FString& serverID, FString IP = FString("0.0.0.0"), int32 port = 8888, bool multicast = false, EReceiveFilterServer receiveFilter = EReceiveFilterServer::E_SAB, FString customServerID = FString(""), int32 maxPacketSize = 65507);

	/**
	*Stop UDP Server
	*@param optionalServerID With one server the field can remain empty. If there are several servers, the ServerID should be entered here or the newest server is automatically taken.
	*/
	UFUNCTION(BlueprintCallable, Category = "SocketServer|UDP")
		void stopUDPServer(FString optionalServerID);
	UFUNCTION(BlueprintCallable, Category = "SocketServer|UDP")
		void stopAllUDPServers();

	/**
	*Sends data back to a client.
	*@param clientSessionIDs
	*@param message
	*@param byteArray
	*@param addLineBreak
	*@param socketType Some Thirdparty software expects the data to be sent back over the same socket, others expects a new socket.
	*@param optionalServerID With one server the field can remain empty. If there are several servers, the ServerID should be entered here or the newest server is automatically taken.
	*/
	UFUNCTION(BlueprintCallable, Category = "SocketServer|UDP", meta = (AutoCreateRefTerm = "byteArray"))
		void socketServerSendUDPMessage(TArray<FString> clientSessionIDs, FString message, TArray<uint8> byteArray, bool addLineBreak, bool asynchronous, ESocketServerUDPSocketType socketType, FString optionalServerID);

	/**
	*Sends data back to a client.
	*@param clientSessionIDs
	*@param message
	*@param byteArray
	*@param addLineBreak
	*@param socketType Some Thirdparty software expects the data to be sent back over the same socket, others expects a new socket.
	*@param optionalServerID With one server the field can remain empty. If there are several servers, the ServerID should be entered here or the newest server is automatically taken.
	*/
	UFUNCTION(BlueprintCallable, Category = "SocketServer|UDP", meta = (AutoCreateRefTerm = "byteArray"))
		void socketServerSendUDPMessageToClient(FString clientSessionID, FString message, TArray<uint8> byteArray, bool addLineBreak, bool asynchronous, ESocketServerUDPSocketType socketType, FString optionalServerID);

	/**
	*If you want to send data directly to a specific destination without getting data back.
	*@param ip
	*@param port
	*@param message
	*@param byteArray
	*@param addLineBreak
	*@param optionalServerID With one server the field can remain empty. If there are several servers, the ServerID should be entered here or the newest server is automatically taken.
	*/
	UFUNCTION(BlueprintCallable, Category = "SocketServer|UDP", meta = (AutoCreateRefTerm = "byteArray"))
		void socketServerSendUDPMessageTo(FString ip, int32 port, FString message, TArray<uint8> byteArray, bool addLineBreak, bool asynchronous, FString optionalServerID);

	

	//TCP
	/**
	*Start TCP Server
	*@param domainOrIP IP or Domain to listen
	*@param port port to listen
	*@param receiveFilter This allows you to decide which data type you want to receive. If you receive files it makes no sense to convert them into a string.
	*@param customServerID Optionally you can assign your own ServerID like "myAuthentificationServer" or "fileServer"
	*/
	UFUNCTION(BlueprintCallable, Category = "SocketServer|TCP")
		void startTCPServer(FString& serverID, FString IP = FString("0.0.0.0"), int32 port = 8888, EReceiveFilterServer receiveFilter = EReceiveFilterServer::E_SAB, FString customServerID = FString(""));
	/**
	*Stop TCP Server
	*@param optionalServerID With one server the field can remain empty. If there are several servers, the ServerID should be entered here or the newest server is automatically taken.
	*/
	UFUNCTION(BlueprintCallable, Category = "SocketServer|TCP")
		void stopTCPServer(FString optionalServerID);
	UFUNCTION(BlueprintCallable, Category = "SocketServer|TCP")
		void stopAllTCPServers();

	//UFUNCTION(BlueprintCallable, Category = "SocketServer|TCP", meta = (AutoCreateRefTerm = "userIdsToDirectoriesMap"))
	//	void startTCPFileserver(FString& serverID, TMap<FString, FString> userIdsToDirectoriesMap, FString IP = FString("0.0.0.0"), int32 port = 9999, FString downloadDirectory = FString("Content/"), EFileFunctionsSocketServerDirectoryType directoryType = EFileFunctionsSocketServerDirectoryType::E_gd, EHTTPSocketServerFileDownloadResumeType ifFileExistThen = EHTTPSocketServerFileDownloadResumeType::E_RESUME, FString customServerID = FString(""), bool onlyWithToken = true);
	//UFUNCTION(BlueprintCallable, Category = "SocketServer|TCP")
	//	void addFileserverToken(FString token, int32 lifeTimeSeconds,bool reusable);
	//UFUNCTION(BlueprintCallable, Category = "SocketServer|TCP")
	//	void removeFileserverToken(FString token);

	/**
	*Send data to clients
	*@param clientSessionIDs array with client sessionIDs
	*@param message data as string
	*@param byteArray data as bytes
	*@param addLineBreak add linebreak to message
	*@param serverID Id of the server you want to send data from
	*@param optionalServerID With one server the field can remain empty. If there are several servers, the ServerID should be entered here or the newest server is automatically taken.
	*/
	UFUNCTION(BlueprintCallable, Category = "SocketServer|TCP", meta = (AutoCreateRefTerm = "byteArray"))
		void socketServerSendTCPMessage(TArray<FString> clientSessionIDs, FString message, TArray<uint8> byteArray, bool addLineBreak = true, FString optionalServerID = FString(""));

	/**
	*Send data to client
	*@param clientSessionID array with client sessionIDs
	*@param message data as string
	*@param byteArray data as bytes
	*@param addLineBreak add linebreak to message
	*@param serverID Id of the server you want to send data from
	*@param optionalServerID With one server the field can remain empty. If there are several servers, the ServerID should be entered here or the newest server is automatically taken.
	*/
	UFUNCTION(BlueprintCallable, Category = "SocketServer|TCP", meta = (AutoCreateRefTerm = "byteArray"))
		void socketServerSendTCPMessageToClient(FString clientSessionID, FString message, TArray<uint8> byteArray, bool addLineBreak = true, FString optionalServerID = FString(""));

	/**
	*Send file to client
	*@param clientSessionID  client sessionID
	*@param directoryWithFileName 
	*@param directoryType 
	*@param serverID Id of the server you want to send file from
	*/
	/*UFUNCTION(BlueprintCallable, Category = "SocketServer|TCP", meta = (AutoCreateRefTerm = "byteArray"))
		void socketServerSendTCPFile(FString clientSessionID, FString directoryWithFileName = FString("Content/image.png"), EFileFunctionsSocketServerDirectoryType directoryType = EFileFunctionsSocketServerDirectoryType::E_gd, int64 resumeFileSize =0,FString serverID = FString(""));*/

	/**
	*Creates a unique ID
	*/
	UFUNCTION(BlueprintCallable, Category = "SocketServer")
		static FString generateUniqueID();

	/**
	*Resolve Domain. Only Domains. Hostnames do not work.
	*@param domain
	*@param useDNSCache Domain and IP are stored in RAM. Starting from the second time the IP is taken from the RAM.
	*@param dnsIP
	*/
	UFUNCTION(BlueprintCallable, Category = "SocketServer")
		UDNSClientSocketServer* resolveDomain(FString domain, bool useDNSCache = true, FString dnsIP = FString("8.8.8.8"));


	/**
	*UE4 uses different socket connections. When Steam is active, Steam Sockets are used for all connections. This leads to problems if you want to use Steam but not Steam Sockets. Therefore you can change the sockets.
	*@param platform Auto =  UE4 decides. Thirdparty platforms (Steam, Playstore etc.) will be used if configured. System = UE4 determines the OS and selects a socket type accordingly.
	*/
	UFUNCTION(BlueprintCallable, Category = "SocketServer")
		static void changeSocketPlatform(ESocketPlatformServer platform);


	/**
	* Checks if a server can listen on IP x and port y.
	*/
	UFUNCTION(BlueprintCallable, Category = "SocketServer")
		bool checkPort(EServerSocketConnectionCheckPortType type, FString ip = FString("0.0.0.0"), int32 port = 8888);

	/**
	* Returns a random port on which the server can listen.
	*/
	UFUNCTION(BlueprintCallable, Category = "SocketServer")
		int32 getRandomPort(EServerSocketConnectionCheckPortType type, FString ip = FString("0.0.0.0"));

	/**
	* 
	*/
	UFUNCTION(BlueprintCallable, Category = "SocketServer|Register")
		void registerClientEvent(FString sessionID, UEventBean*& event);

	UFUNCTION(BlueprintCallable, Category = "SocketServer|Register")
		void unregisterClientEvent(FString sessionID);

	UEventBean* getResiteredClientEvent(FString sessionID);


	static ISocketSubsystem* getSocketSubSystem();



	//void addClientSession(FString key, FClientSocketSession session);
	//void removeClientSession(FString key);
	//TMap<FString, FClientSocketSession> getClientSessions();

	//void startTCPClientHandler(FClientSocketSession session, EReceiveFilterServer receiveFilter);
	//bool isTCPServerRun();
	//void setTCPServerRun(bool run);


private:

	ESocketPlatformServer systemSocketPlatform;
	TMap<FString, UEventBean*> messageEvents;

	TMap<FString, USocketServerPluginUDPServer*> udpServers;
	TMap<FString, USocketServerPluginTCPServer*> tcpServers;


	FString lastUDPServerID;
	FString lastTCPServerID;

};

