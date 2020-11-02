// Copyright 2017-2020 David Romanski (Socke). All Rights Reserved.

#include "SocketServerPluginTCPServer.h"


USocketServerPluginTCPServer::USocketServerPluginTCPServer(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	this->AddToRoot();
}


void USocketServerPluginTCPServer::startTCPServer(IpAndPortStruct ipStructP,FString IPP, int32 portP, EReceiveFilterServer receiveFilterP, FString serverIDP) {
	serverIPArray = ipStructP.ipArray;
	serverIP = IPP;
	serverPort = portP;
	receiveFilter = receiveFilterP;
	serverID = serverIDP;
	serverThread = new FServerTCPThread(this, receiveFilter);
}


void USocketServerPluginTCPServer::startTCPFileServer(IpAndPortStruct ipStructP, FString IPP, int32 portP, FString serverIDP, FString downloadDirP, EHTTPSocketServerFileDownloadResumeType ifFileExistThenP) {
	serverIPArray = ipStructP.ipArray;
	serverIP = IPP;
	serverPort = portP;
	fileServer = true;
	downloadDir = downloadDirP;
	serverID = serverIDP;
	ifFileExistThen = ifFileExistThenP;
	serverThread = new FServerTCPThread(this, receiveFilter);
}


void USocketServerPluginTCPServer::stopTCPServer() {

	run = false;

	TArray<FString> toRemoveSessionKeys;
	for (auto& element : getClientSessions()) {
		toRemoveSessionKeys.Add(element.Key);
	}
	for (int32 i = 0; i < toRemoveSessionKeys.Num(); i++) {
		removeClientSession(toRemoveSessionKeys[i]);
	}
	toRemoveSessionKeys.Empty();

	if (serverThread != nullptr) {
		serverThread->stopThread();
		serverThread = nullptr;
	}

}

void USocketServerPluginTCPServer::sendTCPMessage(TArray<FString> clientSessionIDs, FString message, TArray<uint8> byteArray, bool addLineBreak){
	for (auto& sessionID : clientSessionIDs) {
		FClientSocketSession* sessionPointer = clientSessions.Find(sessionID);
		if (sessionPointer != nullptr) {
			FClientSocketSession session = *sessionPointer;
			if (session.protocol == EServerSocketConnectionProtocol::E_TCP) {

				if (session.sendThread == nullptr) {
					UE_LOG(LogTemp, Warning, TEXT("SimpleSocketServer Plugin: The thread for sending data has not yet been initialized. Data is not sent."));
				}
				else {
					session.sendThread->sendMessage(message, byteArray);
				}
			}
		}
	}
}

void USocketServerPluginTCPServer::sendTCPMessageToClient(FString clientSessionID, FString message, TArray<uint8> byteArray, bool addLineBreak) {
	FClientSocketSession* sessionPointer = clientSessions.Find(clientSessionID);
	if (sessionPointer != nullptr) {
		FClientSocketSession session = *sessionPointer;
		if (session.protocol == EServerSocketConnectionProtocol::E_TCP) {

			if (session.sendThread == nullptr) {
				UE_LOG(LogTemp, Warning, TEXT("SimpleSocketServer Plugin: The thread for sending data has not yet been initialized. Data is not sent."));
			}
			else {
				session.sendThread->sendMessage(message, byteArray);
			}
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("SimpleSocketServer Plugin: Session not found: %s"),*clientSessionID);
	}
}

void USocketServerPluginTCPServer::startTCPDownloadThread(FString clientSessionID, FString directoryWithFileName, int64 resumeFileSize){
	FClientSocketSession* sessionPointer = clientSessions.Find(clientSessionID);
	if (sessionPointer != nullptr) {
		FClientSocketSession session = *sessionPointer;
		if (session.socket == nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("SimpleSocketServer Plugin: Connection for sending file has not found. Data is not sent."));			
		}
		else {
			new FTCPClientSendFileToClientThread(this, session, directoryWithFileName, resumeFileSize);
		}
	}
}

TArray<int32> USocketServerPluginTCPServer::getServerIpArray() {
	return serverIPArray;
}

FString USocketServerPluginTCPServer::getIP() {
	return serverIP;
}

int32 USocketServerPluginTCPServer::getPort() {
	return serverPort;
}

FString USocketServerPluginTCPServer::getServerID() {
	return serverID;
}


void USocketServerPluginTCPServer::initTCPClientThreads(FClientSocketSession sessionP, EReceiveFilterServer receiveFilterP){
	if (fileServer) {
		 new FTCPClientReceiveFilesFromServerThread(this, sessionP, downloadDir);
	}
	else {
		sessionP.recieverThread = new FTCPClientReceiveDataFromServerThread(this, sessionP, receiveFilterP);
	}
	sessionP.sendThread = new FTCPClientSendDataToServerThread(this,sessionP);
	
	addClientSession(sessionP);
}

void USocketServerPluginTCPServer::addClientSession(FClientSocketSession session) {
	if (session.sessionID.IsEmpty() == false) {
		//UE_LOG(LogTemp, Warning, TEXT("ADD Session:%s"), *session.sessionID);
		clientSessions.Add(session.sessionID, session);
	}
}

FClientSocketSession* USocketServerPluginTCPServer::getClientSession(FString key) {
	return clientSessions.Find(key);
}

void USocketServerPluginTCPServer::removeClientSession(FString key) {
	//close client socket
	if (clientSessions.Find(key) != nullptr) {
		FClientSocketSession session = *clientSessions.Find(key);

		if (session.recieverThread != nullptr) {
			session.recieverThread->stopThread();
			session.recieverThread = nullptr;
		}

		if (session.sendThread != nullptr) {
			session.sendThread->stopThread();
			session.sendThread = nullptr;
		}

	}
	if (clientSessions.Remove(key)) {
		//UE_LOG(LogTemp, Warning, TEXT("Remove Session:%s"), *key);
		USocketServerBPLibrary::socketServerBPLibrary->unregisterClientEvent(key);
	}
}

TMap<FString, FClientSocketSession> USocketServerPluginTCPServer::getClientSessions(){
	return clientSessions;
}

EHTTPSocketServerFileDownloadResumeType USocketServerPluginTCPServer::getifFileExistThen(){
	return ifFileExistThen;
}

bool USocketServerPluginTCPServer::isRun(){
	return run;
}
