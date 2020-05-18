// Copyright 2017-2020 David Romanski (Socke). All Rights Reserved.

#include "SocketServerBPLibrary.h"
#include "SocketServer.h"

USocketServerBPLibrary* USocketServerBPLibrary::socketServerBPLibrary;

USocketServerBPLibrary::USocketServerBPLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	socketServerBPLibrary = this;

	//Delegates
	onsocketServerConnectionEventDelegate.AddDynamic(this, &USocketServerBPLibrary::socketServerConnectionEventDelegate);
	onserverReceiveTCPMessageEventDelegate.AddDynamic(this, &USocketServerBPLibrary::serverReceiveTCPMessageEventDelegate);
	onsocketServerUDPConnectionEventDelegate.AddDynamic(this, &USocketServerBPLibrary::socketServerUDPConnectionEventDelegate);
	onserverReceiveUDPMessageEventDelegate.AddDynamic(this, &USocketServerBPLibrary::serverReceiveUDPMessageEventDelegate);
	
}


/*Delegate functions*/
void USocketServerBPLibrary::socketServerConnectionEventDelegate(const EServerSocketConnectionEventType type, const bool success, const FString message, const FString sessionID, const FString serverID) {}
void USocketServerBPLibrary::serverReceiveTCPMessageEventDelegate(const FString sessionID, const FString message, const TArray<uint8>& byteArray, const FString serverID) {}
void USocketServerBPLibrary::serverReceiveTCPFileDownloadEventDelegate(const int32 statusCode, const FString statusMessage, const FString sessionID, const FString serverID, const FString fullPath){}
void USocketServerBPLibrary::serverReceiveTCPFileDownloadProgressEventDelegate(const FSocketServerDownloadFileInfo fileInfo){}
void USocketServerBPLibrary::serverSendTCPFileUploadEventDelegate(const int32 statusCode, const FString statusMessage, const FString sessionID, const FString serverID, const FString fullPath){}
void USocketServerBPLibrary::serverSendTCPFileUploadProgressEventDelegate(const FSocketServerUploadFileInfo fileInfo){}
void USocketServerBPLibrary::socketServerUDPConnectionEventDelegate(const bool success, const FString message, const FString serverID) {}
void USocketServerBPLibrary::serverReceiveUDPMessageEventDelegate(const FString sessionID, const FString message, const TArray<uint8>& byteArray, const FString serverID) {}



USocketServerBPLibrary * USocketServerBPLibrary::getSocketServerTarget() {
	return socketServerBPLibrary;
}


void USocketServerBPLibrary::breakDownloadFileInfo(FSocketServerDownloadFileInfo downloadFileInfo, float& size, float& megaBytesLeft, float& megaBytesReceived, float& percentDownload, float& megaBit, FString& fileName, FString& serverID){
	size = downloadFileInfo.size;
	megaBytesLeft = downloadFileInfo.megaBytesLeft;
	megaBytesReceived = downloadFileInfo.megaBytesReceived;
	percentDownload = downloadFileInfo.percentDownload;
	megaBit = downloadFileInfo.megaBit;
	fileName = downloadFileInfo.fileName;
	serverID = downloadFileInfo.serverID;
}

void USocketServerBPLibrary::breakUploadFileInfo(FSocketServerUploadFileInfo fileUploadInfo, float& size, float& megaBytesLeft, float& megaBytesSend, float& percentUpload, float& megaBit, FString& fileName, FString& serverID) {
	size = fileUploadInfo.size;
	megaBytesLeft = fileUploadInfo.megaBytesLeft;
	megaBytesSend = fileUploadInfo.megaBytesSend;
	percentUpload = fileUploadInfo.percentUpload;
	megaBit = fileUploadInfo.megaBit;
	fileName = fileUploadInfo.fileName;
	serverID = fileUploadInfo.serverID;
}

void USocketServerBPLibrary::serverPluginGetSocketSessionIds(const FString serverID, TArray<FString>& sessionIDs) {
	if (serverID.IsEmpty()) {
		if (udpServers.Find(lastUDPServerID) != nullptr) {
			(*udpServers.Find(lastUDPServerID))->getClientSessions().GetKeys(sessionIDs);
			return;
		}
	}
	else {
		if (udpServers.Find(serverID) != nullptr) {
			(*udpServers.Find(serverID))->getClientSessions().GetKeys(sessionIDs);
			return;
		}
	}

	if (serverID.IsEmpty()) {
		if (tcpServers.Find(lastTCPServerID) != nullptr) {
			(*tcpServers.Find(lastTCPServerID))->getClientSessions().GetKeys(sessionIDs);
		}
	}
	else {
		if (tcpServers.Find(serverID) != nullptr) {
			(*tcpServers.Find(serverID))->getClientSessions().GetKeys(sessionIDs);
		}
	}
}

void USocketServerBPLibrary::serverPluginGetSocketSessionInfoByServerID(const FString serverID, const FString sessionID, bool& sessionFound, FString& IP, int32& port, EServerSocketConnectionProtocol& connectionProtocol) {
	if (sessionID.IsEmpty()) {
		sessionFound = false;
		return;
	}

	FClientSocketSession* sessionPointer = nullptr;

	if (udpServers.Find(serverID) != nullptr) {
		sessionPointer = (*udpServers.Find(serverID))->getClientSession(sessionID);
	}
	else {
		sessionPointer = (*tcpServers.Find(serverID))->getClientSession(sessionID);
	}

	if (sessionPointer != nullptr) {
		FClientSocketSession session = *sessionPointer;
		IP = session.ip;
		port = session.port;
		connectionProtocol = session.protocol;
		sessionFound = true;
	}
	else {
		sessionFound = false;
		IP = "";
		port = -1;
		connectionProtocol = EServerSocketConnectionProtocol::E_NotSet;
	}
}


void USocketServerBPLibrary::serverPluginGetSocketSessionInfo(const FString sessionID, bool &sessionFound, FString &IP, int32 &port, EServerSocketConnectionProtocol &connectionProtocol, FString& serverID) {
	if (sessionID.IsEmpty()) {
		sessionFound = false;
		return;
	}

	sessionFound = false;
	IP = "";
	port = -1;
	connectionProtocol = EServerSocketConnectionProtocol::E_NotSet;

	//udp
	for (auto& element : udpServers) {
		serverPluginGetSocketSessionInfoByServerID(element.Key, sessionID, sessionFound, IP, port, connectionProtocol);
		if (sessionFound) {
			serverID = element.Key;
			break;
		}
	}
	
	//tcp
	if (sessionFound == false) {
		for (auto& element : tcpServers) {
			serverPluginGetSocketSessionInfoByServerID(element.Key, sessionID, sessionFound, IP, port, connectionProtocol);
			if (sessionFound) {
				serverID = element.Key;
				break;
			}
		}
	}
}

void USocketServerBPLibrary::removeSessionAndCloseConnection(FString sessionId, FString serverID) {
	if (serverID.IsEmpty()) {
		if (lastUDPServerID.IsEmpty() == false && udpServers.Find(lastUDPServerID) != nullptr) {
			(*udpServers.Find(lastUDPServerID))->removeClientSession(sessionId);
		}
		if (lastTCPServerID.IsEmpty() == false && tcpServers.Find(lastTCPServerID) != nullptr) {
			(*tcpServers.Find(lastTCPServerID))->removeClientSession(sessionId);
		}
	}
	else {
		if (udpServers.Find(serverID) != nullptr) {
			(*udpServers.Find(serverID))->removeClientSession(sessionId);
		}
		if (serverID.IsEmpty() == false && tcpServers.Find(serverID) != nullptr) {
			(*tcpServers.Find(serverID))->removeClientSession(sessionId);
		}
	}


}

IpAndPortStruct USocketServerBPLibrary::checkIpAndPort(FString IP, int32 port) {

	IpAndPortStruct ipAndPortStruct;

	//simple ip check
	TArray<FString> ipStringArray;
	int32 lineCount = IP.ParseIntoArray(ipStringArray, TEXT("."), true);
	if (lineCount != 4) {
		UE_LOG(LogTemp, Error, TEXT("Invalid IP:%s"), *IP);
		ipAndPortStruct.errorMessage = "Invalid IP: " + IP;
		ipAndPortStruct.success = false;
		return ipAndPortStruct;
	}
	TArray<int32> ipArray = { FCString::Atoi(*ipStringArray[0]), FCString::Atoi(*ipStringArray[1]), FCString::Atoi(*ipStringArray[2]) ,FCString::Atoi(*ipStringArray[3]) };
	if ((ipArray[0] < 0 || ipArray[0] > 255) || (ipArray[1] < 0 || ipArray[1] > 255) || (ipArray[2] < 0 || ipArray[2] > 255) || (ipArray[3] < 0 || ipArray[3] > 255)) {
		UE_LOG(LogTemp, Error, TEXT("Invalid IP:%s"), *IP);
		ipAndPortStruct.errorMessage = "Invalid IP: " + IP;
		ipAndPortStruct.success = false;
		return ipAndPortStruct;
	}

	//simple port check
	if (port < 0 || port > 65535) {
		UE_LOG(LogTemp, Error, TEXT("Invalid port:%i"), port);
		ipAndPortStruct.errorMessage = "Invalid port: " + FString::FromInt(port);
		ipAndPortStruct.success = false;
		return ipAndPortStruct;
	}

	ipAndPortStruct.ipArray = ipArray;
	ipAndPortStruct.port = port;
	ipAndPortStruct.success = true;
	return ipAndPortStruct;
}



//UDP
void USocketServerBPLibrary::stopUDPServer(FString serverID) {
	if (serverID.IsEmpty()) {
		serverID = lastUDPServerID;
	}

	if (serverID.IsEmpty() || udpServers.Find(serverID) == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("ServerID not found: %s"), *serverID);
		return;
	}

	USocketServerPluginUDPServer* udpServer = *udpServers.Find(serverID);
	if (udpServer != nullptr){
		udpServer->stopUDPServer();
	}
	udpServers.Remove(serverID);

	AsyncTask(ENamedThreads::GameThread, [serverID]() {
		USocketServerBPLibrary::socketServerBPLibrary->onsocketServerUDPConnectionEventDelegate.Broadcast(false, "UDP Server stopped.", serverID);
	});
}

void USocketServerBPLibrary::stopAllUDPServers() {

	TArray<FString> toRemoveServerKeys;
	for (auto& element : udpServers) {
		toRemoveServerKeys.Add(element.Key);
	}

	for (auto& element : toRemoveServerKeys) {
		stopUDPServer(element);
	}
}

void USocketServerBPLibrary::startUDPServer(FString& serverID, FString IP, int32 port ,bool multicast, EReceiveFilterServer receiveFilter, FString optionalServerID, int32 maxPacketSize) {

	serverID = optionalServerID;

	if (serverID.IsEmpty()) {
		serverID = FGuid::NewGuid().ToString();
		UE_LOG(LogTemp, Display, TEXT("SimpleSocketServer Plugin: ServerID not set. Generate automatically one :%s"), *serverID);
	}

	if (udpServers.Find(serverID) != nullptr) {
		AsyncTask(ENamedThreads::GameThread, [serverID]() {
			USocketServerBPLibrary::socketServerBPLibrary->onsocketServerUDPConnectionEventDelegate.Broadcast(false, "An server with this serverID is already started.", serverID);
		});
		return;
	}

	IpAndPortStruct ipStruct = checkIpAndPort(IP, port);
	if (ipStruct.success == false) {
		AsyncTask(ENamedThreads::GameThread, [IP,port,serverID]() {
			USocketServerBPLibrary::socketServerBPLibrary->onsocketServerUDPConnectionEventDelegate.Broadcast(false, "Can't start server on: " + IP+":"+FString::FromInt(port),serverID);
		});
		return;
	}

	//check used port
	for (auto& element : udpServers) {
		USocketServerPluginUDPServer* udpServerTmp = element.Value;
		if (udpServerTmp != nullptr) {
			if (udpServerTmp->getIP().Equals(IP) && udpServerTmp->getPort() == port) {
				FString tmpServerID = udpServerTmp->getServerID();
				AsyncTask(ENamedThreads::GameThread, [IP, port, serverID, tmpServerID]() {
					USocketServerBPLibrary::socketServerBPLibrary->onsocketServerUDPConnectionEventDelegate.Broadcast(false, "Can't start server. Address is already used by: " + tmpServerID, serverID);
				});
				return;
			}
		}
	}

	USocketServerPluginUDPServer* udpServer = NewObject<USocketServerPluginUDPServer>(USocketServerPluginUDPServer::StaticClass());
	udpServers.Add(serverID, udpServer);
	udpServer->startUDPServer(ipStruct,IP, port, multicast, receiveFilter, serverID, maxPacketSize);

	lastUDPServerID = serverID;
}


void USocketServerBPLibrary::socketServerSendUDPMessage(TArray<FString> clientSessionIDs, FString message, TArray<uint8> byteArray, bool addLineBreak, bool asynchronous, ESocketServerUDPSocketType socketType, FString serverID) {
	if (message.Len() == 0 && byteArray.Num() == 0)
		return;

	if (serverID.IsEmpty()) {
		serverID = lastUDPServerID;
	}

	if (serverID.IsEmpty() || udpServers.Find(serverID) == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("SimpleSocketServer Plugin: ServerID not found: %s"), *serverID);
		return;
	}

	if (message.Len() > 0 && addLineBreak) {
		message.Append("\r\n");
	}

	USocketServerPluginUDPServer* udpServer = *udpServers.Find(serverID);
	if (udpServer != nullptr) {
		udpServer->sendUDPMessage(clientSessionIDs, message, byteArray, asynchronous, socketType);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("SimpleSocketServer Plugin: Server not found or not started: %s"), *serverID);
		return;
	}
}


void USocketServerBPLibrary::socketServerSendUDPMessageToClient(FString clientSessionID, FString message, TArray<uint8> byteArray, bool addLineBreak, bool asynchronous, ESocketServerUDPSocketType socketType, FString serverID) {
	if (message.Len() == 0 && byteArray.Num() == 0)
		return;

	if (serverID.IsEmpty()) {
		serverID = lastUDPServerID;
	}

	if (serverID.IsEmpty() || udpServers.Find(serverID) == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("SimpleSocketServer Plugin: ServerID not found: %s"), *serverID);
		return;
	}

	if (message.Len() > 0 && addLineBreak) {
		message.Append("\r\n");
	}

	USocketServerPluginUDPServer* udpServer = *udpServers.Find(serverID);
	if (udpServer != nullptr) {
		udpServer->sendUDPMessageToClient(clientSessionID, message, byteArray, asynchronous, socketType);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("SimpleSocketServer Plugin: Server not found or not started: %s"), *serverID);
		return;
	}
}

void USocketServerBPLibrary::socketServerSendUDPMessageTo(FString ip, int32 port, FString message, TArray<uint8> byteArray, bool addLineBreak, bool asynchronous, FString serverID) {
	if (message.Len() == 0 && byteArray.Num() == 0)
		return;

	if (serverID.IsEmpty()) {
		serverID = lastUDPServerID;
	}

	if (serverID.IsEmpty() || udpServers.Find(serverID) == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("SimpleSocketServer Plugin: ServerID not found: %s"), *serverID);
		return;
	}

	if (message.Len() > 0 && addLineBreak) {
		message.Append("\r\n");
	}

	USocketServerPluginUDPServer* udpServer = *udpServers.Find(serverID);
	if (udpServer != nullptr) {
		udpServer->sendUDPMessageTo(ip,port, message, byteArray, asynchronous);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("SimpleSocketServer Plugin: Server not found or not started: %s"), *serverID);
		return;
	}
}



//TCP
void USocketServerBPLibrary::startTCPServer(FString& serverID, FString IP, int32 port, EReceiveFilterServer receiveFilter, FString optionalServerID) {

	serverID = optionalServerID;

	if (serverID.IsEmpty()) {
		serverID = FGuid::NewGuid().ToString();
		UE_LOG(LogTemp, Display, TEXT("SimpleSocketServer Plugin: ServerID not set. Generate automatically one :%s"), *serverID);
	}

	if (tcpServers.Find(serverID) != nullptr) {
		AsyncTask(ENamedThreads::GameThread, [serverID]() {
			USocketServerBPLibrary::socketServerBPLibrary->onsocketServerConnectionEventDelegate.Broadcast(EServerSocketConnectionEventType::E_Server, false, "An server with this serverID is already started.","",serverID);
		});
		return;
	}

	IpAndPortStruct ipStruct = checkIpAndPort(IP, port);
	if (ipStruct.success == false) {
		AsyncTask(ENamedThreads::GameThread, [IP, port, serverID]() {
			USocketServerBPLibrary::socketServerBPLibrary->onsocketServerConnectionEventDelegate.Broadcast(EServerSocketConnectionEventType::E_Server, false, "Can't start server on: " + IP + ":" + FString::FromInt(port), "", serverID);
		});
		return;
	}

	//check used port
	for (auto& element : tcpServers) {
		USocketServerPluginTCPServer* tcpServerTmp = element.Value;
		if (tcpServerTmp != nullptr) {
			if (tcpServerTmp->getIP().Equals(IP) && tcpServerTmp->getPort() == port) {
				FString tmpServerID = tcpServerTmp->getServerID();
				AsyncTask(ENamedThreads::GameThread, [IP, port, serverID, tmpServerID]() {
					USocketServerBPLibrary::socketServerBPLibrary->onsocketServerConnectionEventDelegate.Broadcast(EServerSocketConnectionEventType::E_Server, false, "Can't start server. Address is already used by: " + tmpServerID, "", serverID);
				});
				return;
			}
		}
	}

	USocketServerPluginTCPServer* tcpServer = NewObject<USocketServerPluginTCPServer>(USocketServerPluginTCPServer::StaticClass());
	if (serverID.IsEmpty()) {
		serverID = FGuid::NewGuid().ToString();
		UE_LOG(LogTemp, Warning, TEXT("SimpleSocketServer Plugin: ServerID not set. Generate automatically one :%s"), *serverID);
	}
	tcpServers.Add(serverID, tcpServer);
	tcpServer->startTCPServer(ipStruct, IP, port, receiveFilter, serverID);

	lastTCPServerID = serverID;
}

//void USocketServerBPLibrary::startTCPFileserver(FString& serverID, TMap<FString, FString> userIdsToDirectoriesMap, FString IP, int32 port, FString downloadDirectory, EFileFunctionsSocketServerDirectoryType directoryType, EHTTPSocketServerFileDownloadResumeType ifFileExistThen, FString optionalServerID, bool onlyWithToken){
//
//	serverID = optionalServerID;
//
//	if (serverID.IsEmpty()) {
//		serverID = FGuid::NewGuid().ToString();
//		UE_LOG(LogTemp, Display, TEXT("SimpleSocketServer Plugin: ServerID not set. Generate automatically one :%s"), *serverID);
//	}
//
//	if (UFileFunctionsSocketServer::directoryExists(directoryType, downloadDirectory) == false) {
//		AsyncTask(ENamedThreads::GameThread, [serverID]() {
//			USocketServerBPLibrary::socketServerBPLibrary->onsocketServerConnectionEventDelegate.Broadcast(EServerSocketConnectionEventType::E_Server, false, "Directory does not exist or is not writable.", "", serverID);
//			});
//		return;
//	}
//
//	if (serverID.IsEmpty() == false && tcpServers.Find(serverID) != nullptr) {
//		AsyncTask(ENamedThreads::GameThread, [serverID]() {
//			USocketServerBPLibrary::socketServerBPLibrary->onsocketServerConnectionEventDelegate.Broadcast(EServerSocketConnectionEventType::E_Server, false, "An server with this serverID is already started.", "", serverID);
//			});
//		return;
//	}
//
//	IpAndPortStruct ipStruct = checkIpAndPort(IP, port);
//	if (ipStruct.success == false) {
//		AsyncTask(ENamedThreads::GameThread, [IP, port, serverID]() {
//			USocketServerBPLibrary::socketServerBPLibrary->onsocketServerConnectionEventDelegate.Broadcast(EServerSocketConnectionEventType::E_Server, false, "Can't start server on: " + IP + ":" + FString::FromInt(port), "", serverID);
//			});
//		return;
//	}
//
//	//check used port
//	for (auto& element : tcpServers) {
//		USocketServerPluginTCPServer* tcpServerTmp = element.Value;
//		if (tcpServerTmp != nullptr) {
//			if (tcpServerTmp->getIP().Equals(IP) && tcpServerTmp->getPort() == port) {
//				FString tmpServerID = tcpServerTmp->getServerID();
//				AsyncTask(ENamedThreads::GameThread, [IP, port, serverID, tmpServerID]() {
//					USocketServerBPLibrary::socketServerBPLibrary->onsocketServerConnectionEventDelegate.Broadcast(EServerSocketConnectionEventType::E_Server, false, "Can't start server. Address is already used by: " + tmpServerID, "", serverID);
//					});
//				return;
//			}
//		}
//	}
//
//	USocketServerPluginTCPServer* tcpServer = NewObject<USocketServerPluginTCPServer>(USocketServerPluginTCPServer::StaticClass());
//	if (serverID.IsEmpty()) {
//		serverID = FGuid::NewGuid().ToString();
//		UE_LOG(LogTemp, Warning, TEXT("SimpleSocketServer Plugin: ServerID not set. Generate automatically one :%s"), *serverID);
//	}
//	tcpServers.Add(serverID, tcpServer);
//	tcpServer->startTCPFileServer(ipStruct, IP, port, serverID, UFileFunctionsSocketServer::getCleanDirectory(directoryType, downloadDirectory), ifFileExistThen);
//
//	lastTCPServerID = serverID;
//}
//
//void USocketServerBPLibrary::addFileserverToken(FString token, int32 lifeTimeSeconds, bool reusable)
//{
//}
//
//void USocketServerBPLibrary::removeFileserverToken(FString token)
//{
//}


void USocketServerBPLibrary::stopTCPServer(FString serverID) {

	if (serverID.IsEmpty()) {
		serverID = lastTCPServerID;
	}

	if (serverID.IsEmpty() || tcpServers.Find(serverID) == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("ServerID not found: %s"), *serverID);
		return;
	}

	USocketServerPluginTCPServer* tcpServer = *tcpServers.Find(serverID);
	if (tcpServer != nullptr) {
		tcpServer->stopTCPServer();
	}
	tcpServers.Remove(serverID);
}

void USocketServerBPLibrary::stopAllTCPServers() {

	TArray<FString> toRemoveServerKeys;
	for (auto& element : tcpServers) {
		toRemoveServerKeys.Add(element.Key);
	}

	for (auto& element : toRemoveServerKeys) {
		stopTCPServer(element);
	}
}

void USocketServerBPLibrary::socketServerSendTCPMessage(TArray<FString> clientSessionIDs, FString message, TArray<uint8> byteArray, bool addLineBreak,FString serverID) {

	if (message.Len() == 0 && byteArray.Num() == 0)
		return;

	if (serverID.IsEmpty()) {
		serverID = lastTCPServerID;
	}

	if (serverID.IsEmpty() || tcpServers.Find(serverID) == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("SimpleSocketServer Plugin: ServerID not found: %s"), *serverID);
		return;
	}

	if (message.Len() > 0 && addLineBreak) {
		message.Append("\r\n");
	}

	USocketServerPluginTCPServer* tcpServer = *tcpServers.Find(serverID);
	if (tcpServer != nullptr) {
		tcpServer->sendTCPMessage(clientSessionIDs, message, byteArray, addLineBreak);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("SimpleSocketServer Plugin: Server not found or not started: %s"), *serverID);
		return;
	}
}

void USocketServerBPLibrary::socketServerSendTCPMessageToClient(FString clientSessionID, FString message, TArray<uint8> byteArray, bool addLineBreak, FString serverID) {

	if (message.Len() == 0 && byteArray.Num() == 0)
		return;

	if (serverID.IsEmpty()) {
		serverID = lastTCPServerID;
	}

	if (serverID.IsEmpty() || tcpServers.Find(serverID) == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("SimpleSocketServer Plugin: ServerID not found: %s"), *serverID);
		return;
	}

	if (message.Len() > 0 && addLineBreak) {
		message.Append("\r\n");
	}

	USocketServerPluginTCPServer* tcpServer = *tcpServers.Find(serverID);
	if (tcpServer != nullptr) {
		tcpServer->sendTCPMessageToClient(clientSessionID, message, byteArray, addLineBreak);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("SimpleSocketServer Plugin: Server not found or not started: %s"), *serverID);
		return;
	}
}


//TMap<FString, FClientSocketSession> USocketServerBPLibrary::getClientSessions() {
//	return clientSessions;
//}

//void USocketServerBPLibrary::socketServerSendTCPFile(FString clientSessionID, FString directoryWithFileName, EFileFunctionsSocketServerDirectoryType directoryType, int64 resumeFileSize, FString serverID){
//	if (UFileFunctionsSocketServer::fileExists(directoryType, directoryWithFileName) == false) {
//		AsyncTask(ENamedThreads::GameThread, [clientSessionID, serverID, directoryWithFileName]() {
//			USocketServerBPLibrary::socketServerBPLibrary->onserverSendTCPFileUploadEventDelegate.Broadcast(-11, "File not exsist: " + directoryWithFileName, clientSessionID, serverID);
//		});
//		return;
//	}
//	USocketServerPluginTCPServer* tcpServer = *tcpServers.Find(serverID);
//	if (tcpServer != nullptr) {
//		tcpServer->sendTCPFile(clientSessionID,UFileFunctionsSocketServer::getCleanDirectory(directoryType, directoryWithFileName),resumeFileSize);
//	}
//	else {
//		UE_LOG(LogTemp, Warning, TEXT("SimpleSocketServer Plugin: Server not found or not started: %s"), *serverID);
//		return;
//	}
//	
//}

FString USocketServerBPLibrary::generateUniqueID(){
	return FGuid::NewGuid().ToString();
}

UDNSClientSocketServer* USocketServerBPLibrary::resolveDomain(FString domain, bool useDNSCache, FString dnsIP){
	UDNSClientSocketServer* dnsClient = NewObject<UDNSClientSocketServer>(UDNSClientSocketServer::StaticClass());
	dnsClient->resolveDomain(USocketServerBPLibrary::getSocketSubSystem(),domain, useDNSCache, dnsIP);
	return dnsClient;
}

void USocketServerBPLibrary::changeSocketPlatform(ESocketPlatformServer platform){
		USocketServerBPLibrary::getSocketServerTarget()->systemSocketPlatform = platform;
}

bool USocketServerBPLibrary::checkPort(EServerSocketConnectionCheckPortType type, FString ip, int32 port){
	if (type == EServerSocketConnectionCheckPortType::E_TCP) {
		IpAndPortStruct ipAndPort = checkIpAndPort(ip, port);
		if (!ipAndPort.success) {
			return false;
		}
		FSocket* listenerSocket = nullptr;
		ISocketSubsystem* socketSubSystem = USocketServerBPLibrary::getSocketSubSystem();
		if (socketSubSystem != nullptr) {

			if (socketSubSystem != nullptr) {
				FIPv4Endpoint BoundEndpoint = FIPv4Endpoint(FIPv4Address(ipAndPort.ipArray[0], ipAndPort.ipArray[1], ipAndPort.ipArray[2], ipAndPort.ipArray[3]), ipAndPort.port);
				listenerSocket = socketSubSystem->CreateSocket(NAME_Stream, *FString("USocketServerBPLibraryListenerCheckTCPPort"), BoundEndpoint.ToInternetAddrIPV4()->GetProtocolType());

				if (listenerSocket != nullptr) {
					listenerSocket->SetLinger(false, 0);
	
					if (!listenerSocket->Bind(*BoundEndpoint.ToInternetAddrIPV4())) {
						if (listenerSocket != nullptr) {
							listenerSocket->Close();
							if (socketSubSystem != nullptr)
								socketSubSystem->DestroySocket(listenerSocket);
							listenerSocket = nullptr;
						}
						return false;
					}


					if (!listenerSocket->Listen(1)) {
						if (listenerSocket != nullptr) {
							listenerSocket->Close();
							if (socketSubSystem != nullptr)
								socketSubSystem->DestroySocket(listenerSocket);
							listenerSocket = nullptr;
						}
						return false;
					}
				}
			}
		}
		if (listenerSocket == nullptr) {
			return false;
		}
		else {			
			listenerSocket->Close();
			if (socketSubSystem != nullptr)
				socketSubSystem->DestroySocket(listenerSocket);
			listenerSocket = nullptr;
			return true;
		}
	}
	else {
		ISocketSubsystem* socketSubSystem = USocketServerBPLibrary::getSocketSubSystem();
		FSocket* listenerSocket = nullptr;
		if (socketSubSystem != nullptr) {
			TSharedPtr<class FInternetAddr> addr = socketSubSystem->CreateInternetAddr();
			bool validIP = true;
			addr->SetPort(port);
			addr->SetIp(*ip, validIP);

			if (!validIP) {
				return false;
			}
			FString socketName;
			listenerSocket = socketSubSystem->CreateSocket(NAME_DGram, *socketName, addr->GetProtocolType());
			if (listenerSocket == nullptr) {
				return false;
			}
			listenerSocket->SetReuseAddr();
			listenerSocket->SetNonBlocking();
			if (!listenerSocket->Bind(*addr)) {
				return false;
			}
		}

		if (listenerSocket == nullptr) {
			return false;
		}
		else {
			listenerSocket->Close();
			if (socketSubSystem != nullptr)
				socketSubSystem->DestroySocket(listenerSocket);
			listenerSocket = nullptr;
			return true;
		}
	}
	return false;
}

int32 USocketServerBPLibrary::getRandomPort(EServerSocketConnectionCheckPortType type, FString ip){
	int32 port = 0;
	if (type == EServerSocketConnectionCheckPortType::E_TCP) {
		IpAndPortStruct ipAndPort = checkIpAndPort(ip, port);
		if (!ipAndPort.success) {
			return 0;
		}
		FSocket* listenerSocket = nullptr;
		ISocketSubsystem* socketSubSystem = USocketServerBPLibrary::getSocketSubSystem();
		if (socketSubSystem != nullptr) {

			if (socketSubSystem != nullptr) {
				FIPv4Endpoint BoundEndpoint = FIPv4Endpoint(FIPv4Address(ipAndPort.ipArray[0], ipAndPort.ipArray[1], ipAndPort.ipArray[2], ipAndPort.ipArray[3]), ipAndPort.port);
				listenerSocket = socketSubSystem->CreateSocket(NAME_Stream, *FString("USocketServerBPLibraryListenerCheckTCPPort"), BoundEndpoint.ToInternetAddrIPV4()->GetProtocolType());

				if (listenerSocket != nullptr) {
					listenerSocket->SetLinger(false, 0);

					if (!listenerSocket->Bind(*BoundEndpoint.ToInternetAddrIPV4())) {
						if (listenerSocket != nullptr) {
							listenerSocket->Close();
							if (socketSubSystem != nullptr)
								socketSubSystem->DestroySocket(listenerSocket);
							listenerSocket = nullptr;
						}
						return 0;
					}


					if (!listenerSocket->Listen(1)) {
						if (listenerSocket != nullptr) {
							listenerSocket->Close();
							if (socketSubSystem != nullptr)
								socketSubSystem->DestroySocket(listenerSocket);
							listenerSocket = nullptr;
						}
						return 0;
					}
				}
			}
		}
		if (listenerSocket == nullptr) {
			return 0;
		}
		else {
			port = listenerSocket->GetPortNo();
			listenerSocket->Close();
			if (socketSubSystem != nullptr)
				socketSubSystem->DestroySocket(listenerSocket);
			listenerSocket = nullptr;
			return port;
		}
	}
	else {
		ISocketSubsystem* socketSubSystem = USocketServerBPLibrary::getSocketSubSystem();
		FSocket* listenerSocket = nullptr;
		if (socketSubSystem != nullptr) {
			TSharedPtr<class FInternetAddr> addr = socketSubSystem->CreateInternetAddr();
			bool validIP = true;
			addr->SetPort(port);
			addr->SetIp(*ip, validIP);

			if (!validIP) {
				return 0;
			}
			FString socketName;
			listenerSocket = socketSubSystem->CreateSocket(NAME_DGram, *socketName, addr->GetProtocolType());
			if (listenerSocket == nullptr) {
				return 0;
			}
			listenerSocket->SetReuseAddr();
			listenerSocket->SetNonBlocking();
			if (!listenerSocket->Bind(*addr)) {
				return 0;
			}
		}

		if (listenerSocket == nullptr) {
			return 0;
		}
		else {
			port = listenerSocket->GetPortNo();
			listenerSocket->Close();
			if (socketSubSystem != nullptr)
				socketSubSystem->DestroySocket(listenerSocket);
			listenerSocket = nullptr;
			return port;
		}
	}
	return 0;
}

void USocketServerBPLibrary::registerClientEvent(FString sessionID, UEventBean*& event){
	UPROPERTY()
		UEventBean* eventBean = NewObject<UEventBean>(UEventBean::StaticClass());
	eventBean->AddToRoot();
	messageEvents.Add(sessionID, eventBean);
	event = eventBean;
}

void USocketServerBPLibrary::unregisterClientEvent(FString sessionID){
	if (sessionID.IsEmpty())
		return;
	if (messageEvents.Find(sessionID) != nullptr) {
		UEventBean* event = *messageEvents.Find(sessionID);
		if (event != nullptr) {
			event->RemoveFromRoot();
		}
		messageEvents.Remove(sessionID);
	}
}

UEventBean* USocketServerBPLibrary::getResiteredClientEvent(FString sessionID){
	if (sessionID.IsEmpty())
		return nullptr;
	if (messageEvents.Find(sessionID) != nullptr) {
		return *messageEvents.Find(sessionID);
	}
	return nullptr;
}

ISocketSubsystem * USocketServerBPLibrary::getSocketSubSystem(){
	switch (USocketServerBPLibrary::getSocketServerTarget()->systemSocketPlatform)
	{
	case ESocketPlatformServer::E_SSS_SYSTEM:
		//UE_LOG(LogTemp, Display, TEXT("E_SSS_SYSTEM"));
		return ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	case ESocketPlatformServer::E_SSS_WINDOWS:
		//UE_LOG(LogTemp, Display, TEXT("E_SSS_WINDOWS"));
		return ISocketSubsystem::Get(FName(TEXT("WINDOWS")));
	case ESocketPlatformServer::E_SSS_MAC:
		//UE_LOG(LogTemp, Display, TEXT("E_SSS_MAC"));
		return ISocketSubsystem::Get(FName(TEXT("MAC")));
	case ESocketPlatformServer::E_SSS_IOS:
		//UE_LOG(LogTemp, Display, TEXT("E_SSS_IOS"));
		return ISocketSubsystem::Get(FName(TEXT("IOS")));
	case ESocketPlatformServer::E_SSS_UNIX:
		//UE_LOG(LogTemp, Display, TEXT("E_SSS_UNIX"));
		return ISocketSubsystem::Get(FName(TEXT("UNIX")));
	case ESocketPlatformServer::E_SSS_ANDROID:
		//UE_LOG(LogTemp, Display, TEXT("E_SSS_ANDROID"));
		return ISocketSubsystem::Get(FName(TEXT("ANDROID")));
	case ESocketPlatformServer::E_SSS_PS4:
		//UE_LOG(LogTemp, Display, TEXT("E_SSS_PS4"));
		return ISocketSubsystem::Get(FName(TEXT("PS4")));
	case ESocketPlatformServer::E_SSS_XBOXONE:
		//UE_LOG(LogTemp, Display, TEXT("E_SSS_XBOXONE"));
		return ISocketSubsystem::Get(FName(TEXT("XBOXONE")));
	case ESocketPlatformServer::E_SSS_HTML5:
		//UE_LOG(LogTemp, Display, TEXT("E_SSS_HTML5"));
		return ISocketSubsystem::Get(FName(TEXT("HTML5")));
	case ESocketPlatformServer::E_SSS_SWITCH:
		//UE_LOG(LogTemp, Display, TEXT("E_SSS_SWITCH"));
		return ISocketSubsystem::Get(FName(TEXT("SWITCH")));
	case ESocketPlatformServer::E_SSS_DEFAULT:
		//UE_LOG(LogTemp, Display, TEXT("E_SSS_DEFAULT"));
		return ISocketSubsystem::Get();
	default:
		//pUE_LOG(LogTemp, Display, TEXT("DEFAULT"));
		return ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	}	
}

//void USocketServerBPLibrary::addClientSession(FString key, FClientSocketSession session) {
//	clientSessions.Add(key, session);
//}
//
//void USocketServerBPLibrary::removeClientSession(FString key) {
//	FClientSocketSession* sessionPointer = clientSessions.Find(key);	
//	if (sessionPointer != nullptr) {
//		FClientSocketSession session = *sessionPointer;
//		
//		if (session.protocol == EServerSocketConnectionProtocol::E_TCP) {
//			if (session.TCPClientSendDataToServerThread != nullptr && session.TCPClientSendDataToServerThread->isRun()) {
//				session.TCPClientSendDataToServerThread->stopThread();
//			}
//			FSocket * socket = session.socket;
//			if (socket != nullptr) {
//				socket->Close();
//			}
//		}
//		clientSessions.Remove(key);
//	}
//	
//}

//void USocketServerBPLibrary::startTCPClientHandler(FClientSocketSession session, EReceiveFilterServer receiveFilter) {
//	new FClientConnectionThread(session, receiveFilter);
//}
//
//bool USocketServerBPLibrary::isTCPServerRun() {
//	return tcpServerRun;
//}
//
//void USocketServerBPLibrary::setTCPServerRun(bool run) {
//	tcpServerRun = run;
//}
