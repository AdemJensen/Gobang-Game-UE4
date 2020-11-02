// Copyright 2017-2020 David Romanski (Socke). All Rights Reserved.

#include "SocketServerPluginUDPServer.h"


USocketServerPluginUDPServer::USocketServerPluginUDPServer(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	this->AddToRoot();
}


void USocketServerPluginUDPServer::startUDPServer(IpAndPortStruct ipStructP,FString IPP, int32 portP, bool multicastP,
	EReceiveFilterServer receiveFilterP, FString serverIDP, int32 maxPacketSizeP) {

	serverIPArray = ipStructP.ipArray;
	serverIP = IPP;
	serverPort = portP;
	receiveFilter = receiveFilterP;
	serverID = serverIDP;
	maxPacketSize = maxPacketSizeP;
	if (maxPacketSize < 1 || maxPacketSize > 65507)
		maxPacketSize = 65507;
	serverThread = new FServerUDPThread(this, multicastP);
	sendThread = new  FUDPClientSendDataToServerThread(this);

}

void USocketServerPluginUDPServer::stopUDPServer() {

	TArray<FString> toRemoveSessionKeys;
	for (auto& element : getClientSessions()) {
		toRemoveSessionKeys.Add(element.Key);
	}
	for (int32 i = 0; i < toRemoveSessionKeys.Num(); i++) {
		removeClientSession(toRemoveSessionKeys[i]);
	}
	toRemoveSessionKeys.Empty();

	if (sendThread != nullptr) {
		sendThread->stopThread();
		sendThread = nullptr;
	}

	if (serverThread != nullptr) {
		serverThread = nullptr;
	}

	if (socketReceiver != nullptr) {
		socketReceiver->Stop();
		socketReceiver->Exit();
		socketReceiver = nullptr;
	}

	if (socket && socket != nullptr) {
		socket->Close();
		socket = nullptr;
	}
}

void USocketServerPluginUDPServer::sendUDPMessage(TArray<FString> clientSessionIDs, FString message, TArray<uint8> byteArray, bool asynchronous, ESocketServerUDPSocketType socketType) {
	for (auto& sessionID : clientSessionIDs) {
		FClientSocketSession* sessionPointer = clientSessions.Find(sessionID);
		if (sessionPointer != nullptr) {
			FClientSocketSession session = *sessionPointer;
			if (session.protocol == EServerSocketConnectionProtocol::E_UDP) {


				FSocket* socketUDP = socket;
				if (socketUDP == nullptr || socketType == ESocketServerUDPSocketType::E_SSS_CLIENT)
					socketUDP = session.socket;

				if (asynchronous) {
					sendThread->sendMessage(session.ip, session.port, message, byteArray, socketUDP);
					return;
				}

				int32 sent = 0;

				TSharedRef<FInternetAddr> addr = USocketServerBPLibrary::getSocketSubSystem()->CreateInternetAddr();
				bool bIsValid;
				addr->SetIp(*session.ip, bIsValid);
				addr->SetPort(session.port);
				if (bIsValid) {
					if (byteArray.Num() > 0) {
						sendBytes(socketUDP, byteArray, sent, addr);
					}

					if (message.Len() > 0) {
						FTCHARToUTF8 Convert(*message);
						byteArray.Append((uint8*)Convert.Get(), Convert.Length());
						sendBytes(socketUDP, byteArray, sent, addr);
					}
				}
				else {
					UE_LOG(LogTemp, Warning, TEXT("SimpleSocketServer Plugin: Can't send data. Wrong adress."));
				}

			}
		}
		else {
			FString serverIDGlobal = serverID;
			AsyncTask(ENamedThreads::GameThread, [sessionID, serverIDGlobal]() {
				USocketServerBPLibrary::socketServerBPLibrary->onsocketServerUDPConnectionEventDelegate.Broadcast(false, "Can't send message. SessionID not found on this server: " + sessionID, serverIDGlobal);
			});
		}
	}
}


void USocketServerPluginUDPServer::sendUDPMessageToClient(FString clientSessionID, FString message, TArray<uint8> byteArray, bool asynchronous, ESocketServerUDPSocketType socketType) {

	FClientSocketSession* sessionPointer = clientSessions.Find(clientSessionID);
	if (sessionPointer != nullptr) {
		FClientSocketSession session = *sessionPointer;
		if (session.protocol == EServerSocketConnectionProtocol::E_UDP) {
			
			FSocket* socketUDP = socket;
			if (socketUDP == nullptr || socketType == ESocketServerUDPSocketType::E_SSS_CLIENT)
				socketUDP = session.socket;

			if (asynchronous) {
				sendThread->sendMessage(session.ip, session.port, message, byteArray, socketUDP);
				return;
			}

			int32 sent = 0;

			TSharedRef<FInternetAddr> addr = USocketServerBPLibrary::getSocketSubSystem()->CreateInternetAddr();
			bool bIsValid;
			addr->SetIp(*session.ip, bIsValid);
			addr->SetPort(session.port);
			if (bIsValid) {

				if (byteArray.Num() > 0) {
					sendBytes(socketUDP, byteArray, sent, addr);
				}

				if (message.Len() > 0) {
					FTCHARToUTF8 Convert(*message);
					byteArray.Append((uint8*)Convert.Get(), Convert.Length());
					sendBytes(socketUDP, byteArray, sent, addr);
				}

				
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("SimpleSocketServer Plugin: Can't send data. Wrong adress."));
			}

		}
	}
	else {
		FString serverIDGlobal = serverID;
		AsyncTask(ENamedThreads::GameThread, [clientSessionID, serverIDGlobal]() {
			USocketServerBPLibrary::socketServerBPLibrary->onsocketServerUDPConnectionEventDelegate.Broadcast(false, "Can't send message. SessionID not found on this server: " + clientSessionID, serverIDGlobal);
		});
	}

}


void USocketServerPluginUDPServer::sendUDPMessageTo(FString ip, int32 port, FString message, TArray<uint8> byteArray, bool asynchronous) {
	int32 sent = 0;

	FSocket* socketUDP = socket;
	if (socketUDP == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("SimpleSocketServer Plugin: Can't send data. Connection broken. Restart your UDP server %s"),*serverID);
		return;
	}

	if (asynchronous) {
		sendThread->sendMessage(ip,port,message, byteArray,socketUDP);
		return;
	}

	TSharedRef<FInternetAddr> addr = USocketServerBPLibrary::getSocketSubSystem()->CreateInternetAddr();
	bool bIsValid;
	addr->SetIp(*ip, bIsValid);
	addr->SetPort(port);
	if (bIsValid) {

		if (byteArray.Num() > 0) {
			sendBytes(socketUDP, byteArray, sent, addr);
		}

		if (message.Len() > 0) {
			FTCHARToUTF8 Convert(*message);
			byteArray.Append((uint8*)Convert.Get(), Convert.Length());
			sendBytes(socketUDP, byteArray, sent, addr);
		}
		
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("SimpleSocketServer Plugin: Can't send data. Wrong adress."));
	}
}


void USocketServerPluginUDPServer::UDPReceiver(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt) {

	FString sessionID = EndPt.ToString();
	FClientSocketSession* sessionPointer = clientSessions.Find(sessionID);
	if (sessionPointer == nullptr) {
		TSharedRef<FInternetAddr> remoteAddress = EndPt.ToInternetAddr();

		FString socketName;
		FSocket* receiverSocket = FUdpSocketBuilder(*socketName);

		//create and save session
		FClientSocketSession session;
		session.sessionID = sessionID;
		session.serverID = serverID;
		session.ip = remoteAddress.Get().ToString(false);
		session.port = remoteAddress.Get().GetPort();
		session.socket = receiverSocket;
		session.protocol = EServerSocketConnectionProtocol::E_UDP;
		addClientSession(session);
	}
	TArray<uint8> byteArray;
	if (receiveFilter == EReceiveFilterServer::E_SAB || receiveFilter == EReceiveFilterServer::E_B) {
		byteArray.Append(ArrayReaderPtr->GetData(), ArrayReaderPtr->Num());
	}
	
	FString recvMessage;
	if (receiveFilter == EReceiveFilterServer::E_SAB || receiveFilter == EReceiveFilterServer::E_S) {
		char* Data = (char*)ArrayReaderPtr->GetData();
		Data[ArrayReaderPtr->Num()] = '\0';
		recvMessage = FString(UTF8_TO_TCHAR(Data));
	}

	FString serverIDGlobal = serverID;
	//switch to gamethread
	AsyncTask(ENamedThreads::GameThread, [recvMessage, sessionID, byteArray, serverIDGlobal]() {
		USocketServerBPLibrary::socketServerBPLibrary->onserverReceiveUDPMessageEventDelegate.Broadcast(sessionID, recvMessage, byteArray, serverIDGlobal);
		if (USocketServerBPLibrary::socketServerBPLibrary->getResiteredClientEvent(sessionID) != nullptr) {
			USocketServerBPLibrary::socketServerBPLibrary->getResiteredClientEvent(sessionID)->onregisteredEventDelegate.Broadcast(recvMessage, byteArray);
		}
	});
	byteArray.Empty();
	recvMessage.Empty();
}


TArray<int32> USocketServerPluginUDPServer::getServerIpArray() {
	return serverIPArray;
}

FString USocketServerPluginUDPServer::getIP(){
	return serverIP;
}

int32 USocketServerPluginUDPServer::getPort() {
	return serverPort;
}

void USocketServerPluginUDPServer::setSocketReceiver(FUdpSocketReceiver* socketReceiverP, FSocket* socketP) {
	socketReceiver = socketReceiverP;
	socket = socketP;
}

FUdpSocketReceiver* USocketServerPluginUDPServer::getSocketReceiver() {
	return socketReceiver;
}

FSocket* USocketServerPluginUDPServer::getSocket() {
	return socket;
}

FString USocketServerPluginUDPServer::getServerID(){
	return serverID;
}

void USocketServerPluginUDPServer::addClientSession(FClientSocketSession session){
	if (session.sessionID.IsEmpty() == false) {
		//UE_LOG(LogTemp, Warning, TEXT("ADD Session:%s"), *session.sessionID);
		clientSessions.Add(session.sessionID, session);
	}
}

FClientSocketSession* USocketServerPluginUDPServer::getClientSession(FString key){
	return clientSessions.Find(key);
}

void USocketServerPluginUDPServer::removeClientSession(FString key){
	if (clientSessions.Remove(key)) {
		//UE_LOG(LogTemp, Warning, TEXT("Remove Session:%s"), *key);
		USocketServerBPLibrary::socketServerBPLibrary->unregisterClientEvent(key);
	}
}

TMap<FString, FClientSocketSession> USocketServerPluginUDPServer::getClientSessions(){
	return clientSessions;
}

void USocketServerPluginUDPServer::sendBytes(FSocket*& socketP, TArray<uint8>& byteArray, int32& sent, TSharedRef<FInternetAddr>& addr){
	if (byteArray.Num() > maxPacketSize) {
		TArray<uint8> byteArrayTemp;
		for (int32 i = 0; i < byteArray.Num(); i++) {
			byteArrayTemp.Add(byteArray[i]);
			if (byteArrayTemp.Num() == maxPacketSize) {
				sent = 0;
				socketP->SendTo(byteArrayTemp.GetData(), byteArrayTemp.Num(), sent, *addr);
				byteArrayTemp.Empty();
			}
		}
		if (byteArrayTemp.Num() > 0) {
			sent = 0;
			socketP->SendTo(byteArrayTemp.GetData(), byteArrayTemp.Num(), sent, *addr);
			byteArrayTemp.Empty();
		}
	}
	else {
		sent = 0;
		socketP->SendTo(byteArray.GetData(), byteArray.Num(), sent, *addr);
	}

	byteArray.Empty();
}
