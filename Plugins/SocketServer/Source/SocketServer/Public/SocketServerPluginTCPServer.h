// Copyright 2017-2020 David Romanski (Socke). All Rights Reserved.
#pragma once

#include "SocketServer.h"
#include "SocketServerPluginTCPServer.generated.h"


class USocketServerBPLibrary;
class FServerTCPThread;
class FTCPClientReceiveFilesFromServerThread;
class FTCPClientSendFileToClientThread;

UCLASS(Blueprintable, BlueprintType)
class USocketServerPluginTCPServer : public UObject
{
	GENERATED_UCLASS_BODY()

public:

	void startTCPServer(IpAndPortStruct ipStructP,FString IP, int32 port, EReceiveFilterServer receiveFilter, FString serverID);
	void startTCPFileServer(IpAndPortStruct ipStructP, FString IP, int32 port, FString serverID, FString downloadDir, EHTTPSocketServerFileDownloadResumeType ifFileExistThen);
	void stopTCPServer();
	void sendTCPMessage(TArray<FString> clientSessionIDs, FString message, TArray<uint8> byteArray, bool addLineBreak);
	void sendTCPMessageToClient(FString clientSessionID, FString message, TArray<uint8> byteArray, bool addLineBreak);
	void startTCPDownloadThread(FString clientSessionID, FString directoryWithFileName, int64 resumeFileSize);

	TArray<int32> getServerIpArray();
	FString getIP();
	int32 getPort();
	FString getServerID();

	void initTCPClientThreads(FClientSocketSession session, EReceiveFilterServer receiveFilter);

	void addClientSession(FClientSocketSession session);
	FClientSocketSession* getClientSession(FString key);
	void removeClientSession(FString key);
	TMap<FString, FClientSocketSession> getClientSessions();
	EHTTPSocketServerFileDownloadResumeType getifFileExistThen();

	bool isRun();

private:
	bool run = true;
	FString serverID;
	TArray<int32> serverIPArray;
	FString serverIP;
	int32  serverPort = -1;
	EReceiveFilterServer receiveFilter;
	bool fileServer = false;
	FString downloadDir;

	FServerTCPThread* serverThread = nullptr;
	EHTTPSocketServerFileDownloadResumeType ifFileExistThen;

	TMap<FString, FClientSocketSession> clientSessions;
};


/*SERVER start and create client sockets asynchronous Thread*/
class FServerTCPThread : public FRunnable {

public:

	FServerTCPThread(USocketServerPluginTCPServer* tcpServerP, EReceiveFilterServer receiveFilterP) :
		tcpServer(tcpServerP),
		receiveFilter(receiveFilterP) {
		FString threadName = "FServerTCPThread_" + FGuid::NewGuid().ToString();
		thread = FRunnableThread::Create(this, *threadName, 0, EThreadPriority::TPri_Normal);
	}

	virtual uint32 Run() override {

		TArray<int32> serverIPArray = tcpServer->getServerIpArray();
		int32 port = tcpServer->getPort();
		FString serverID = tcpServer->getServerID();

		bool createServer = true;
		FSocket* listenerSocket = nullptr;
		ISocketSubsystem* socketSubSystem = USocketServerBPLibrary::getSocketSubSystem();
		if (socketSubSystem != nullptr){

			if (socketSubSystem != nullptr){
				FIPv4Endpoint BoundEndpoint = FIPv4Endpoint(FIPv4Address(serverIPArray[0], serverIPArray[1], serverIPArray[2], serverIPArray[3]), port);
				listenerSocket = socketSubSystem->CreateSocket(NAME_Stream, *FString("USocketServerBPLibraryListenerSocket"), BoundEndpoint.ToInternetAddrIPV4()->GetProtocolType());

				if (listenerSocket != nullptr){
					listenerSocket->SetLinger(false, 0);	

					if (!listenerSocket->Bind(*BoundEndpoint.ToInternetAddrIPV4())) {
						if (listenerSocket != nullptr) {
							listenerSocket->Close();
							if (socketSubSystem != nullptr)
								socketSubSystem->DestroySocket(listenerSocket);
							listenerSocket = nullptr;
							thread = nullptr;
							UE_LOG(LogTemp, Error, TEXT("(211) TCP Server not started. Can't bind %i.%i.%i.%i:%i. Please check IP,Port or your firewall."), serverIPArray[0], serverIPArray[1], serverIPArray[2], serverIPArray[3], port);
						}
						createServer = false;
					}


					if (createServer && !listenerSocket->Listen(8)) {
						if (listenerSocket != nullptr) {
							listenerSocket->Close();
							if (socketSubSystem != nullptr)
								socketSubSystem->DestroySocket(listenerSocket);
							listenerSocket = nullptr;
							thread = nullptr;
							UE_LOG(LogTemp, Error, TEXT("(212) TCP Server not started. Can't listen on %i.%i.%i.%i:%i. Please check IP,Port or your firewall."), serverIPArray[0], serverIPArray[1], serverIPArray[2], serverIPArray[3], port);
						}
						createServer = false;
					}
				}
			}
		}



		if (!createServer || listenerSocket == nullptr) {
			UE_LOG(LogTemp, Error, TEXT("(210) TCP Server not started on %i.%i.%i.%i:%i. Please check IP,Port or your firewall."), serverIPArray[0], serverIPArray[1], serverIPArray[2], serverIPArray[3], port);
			AsyncTask(ENamedThreads::GameThread, [serverID]() {
				USocketServerBPLibrary::socketServerBPLibrary->onsocketServerConnectionEventDelegate.Broadcast(EServerSocketConnectionEventType::E_Server, false, "TCP Server not started. Please check IP,Port or your firewall.", TEXT(""),serverID);
				USocketServerBPLibrary::getSocketServerTarget()->stopTCPServer(serverID);
			});
		}
		else {
			//switch to gamethread
			AsyncTask(ENamedThreads::GameThread, [serverID]() {
				USocketServerBPLibrary::socketServerBPLibrary->onsocketServerConnectionEventDelegate.Broadcast(EServerSocketConnectionEventType::E_Server, true, "TCP Server started.", TEXT(""), serverID);
			});

			while (run) {
				bool pending;
				listenerSocket->WaitForPendingConnection(pending, FTimespan::FromSeconds(1));
				if (pending) {
					//UE_LOG(LogTemp, Display, TEXT("TCP Client: Pending connection"));

					FClientSocketSession session;
					session.sessionID = FGuid::NewGuid().ToString();
					session.serverID = serverID;

					TSharedRef<FInternetAddr> remoteAddress = USocketServerBPLibrary::getSocketSubSystem()->CreateInternetAddr();
					FSocket* clientSocket = listenerSocket->Accept(*remoteAddress, session.sessionID);

					session.ip = remoteAddress.Get().ToString(false);
					session.port = remoteAddress.Get().GetPort();
					session.socket = clientSocket;
					session.protocol = EServerSocketConnectionProtocol::E_TCP;
					tcpServer->addClientSession(session);
					tcpServer->initTCPClientThreads(session, receiveFilter);
				}
			}
		}


		if (listenerSocket && listenerSocket != nullptr) {
			listenerSocket->Close();
			FPlatformProcess::Sleep(1);
			if (socketSubSystem != nullptr)
				socketSubSystem->DestroySocket(listenerSocket);
		}

		listenerSocket = nullptr;
		thread = nullptr;

		//switch to gamethread
		AsyncTask(ENamedThreads::GameThread, [serverID]() {
			USocketServerBPLibrary::socketServerBPLibrary->onsocketServerConnectionEventDelegate.Broadcast(EServerSocketConnectionEventType::E_Server, false, "TCP Server stopped. Depending on the operating system it can take some time until the port is free again.", TEXT(""),serverID);
		});
		return 0;
	}

	void stopThread() {
		run = false;
	}


protected:
	USocketServerPluginTCPServer* tcpServer;
	EReceiveFilterServer	receiveFilter;
	FRunnableThread* thread = nullptr;
	bool run = true;
};


/*Send data asynchronous Thread*/
class FTCPClientSendDataToServerThread : public FRunnable {

public:

	FTCPClientSendDataToServerThread(USocketServerPluginTCPServer* tcpServerP, FClientSocketSession sessionP) :
		tcpServer(tcpServerP),
		session(sessionP) {
		FString threadName = "FTCPClientSendDataToServerThread_" + FGuid::NewGuid().ToString();
		thread = FRunnableThread::Create(this, *threadName, 0, EThreadPriority::TPri_Normal);
	}

	virtual uint32 Run() override {
		FString serverID = tcpServer->getServerID();
		socket = session.socket;
		FString sessionID = session.sessionID;

		while (run) {

			if (thread == nullptr) {
				FPlatformProcess::Sleep(0.1);
				continue;
			}

			if (socket == nullptr) {
				UE_LOG(LogTemp, Error, TEXT("Socket not found."));
				return 0;
			}


			// try to connect to the server
			if (socket == nullptr || run == false) {
				UE_LOG(LogTemp, Error, TEXT("Connection not exist."));
				//switch to gamethread
				AsyncTask(ENamedThreads::GameThread, [sessionID, serverID]() {
					USocketServerBPLibrary::socketServerBPLibrary->onsocketServerConnectionEventDelegate.Broadcast(EServerSocketConnectionEventType::E_Client, false, "Connection not exist", sessionID, serverID);
					});
				return 0;
			}


			int32 sent = 0;
			if (socket != nullptr && socket->GetConnectionState() == ESocketConnectionState::SCS_Connected) {
				while (messageQueue.IsEmpty() == false) {
					FString m;
					messageQueue.Dequeue(m);
					FTCHARToUTF8 Convert(*m);
					sent = 0;
					socket->Send((uint8*)Convert.Get(), Convert.Length(), sent);
				}

				while (byteArrayQueue.IsEmpty() == false) {
					TArray<uint8> ba;
					byteArrayQueue.Dequeue(ba);
					sent = 0;
					socket->Send(ba.GetData(), ba.Num(), sent);
					ba.Empty();
				}
			}
			else {
				//UE_LOG(LogTemp, Error, TEXT("Connection lost"));
				AsyncTask(ENamedThreads::GameThread, [sessionID, serverID]() {
					USocketServerBPLibrary::socketServerBPLibrary->onsocketServerConnectionEventDelegate.Broadcast(EServerSocketConnectionEventType::E_Client, false, "Connection lost", sessionID, serverID);
				});
			}

			if (run) {
				pauseThread(true);
				//workaround. suspend do not work on all platforms. lets sleep
				while (paused && run) {
					FPlatformProcess::Sleep(0.01);
				}
			}
		}

		run = false;
		thread = nullptr;


		return 0;
	}

	FRunnableThread* getThread() {
		return thread;
	}

	void setThread(FRunnableThread* threadP) {
		thread = threadP;
	}

	void stopThread() {
		run = false;
		if (thread != nullptr) {
			pauseThread(false);
		}
	}

	bool isRun() {
		return run;
	}


	void setMessage(FString messageP, TArray<uint8> byteArrayP) {
		if (messageP.Len() > 0)
			messageQueue.Enqueue(messageP);
		if (byteArrayP.Num() > 0)
			byteArrayQueue.Enqueue(byteArrayP);
	}

	void sendMessage(FString messageP, TArray<uint8> byteArrayP) {
		if (messageP.Len() > 0)
			messageQueue.Enqueue(messageP);
		if (byteArrayP.Num() > 0)
			byteArrayQueue.Enqueue(byteArrayP);
		pauseThread(false);
	}



	void pauseThread(bool pause) {
		paused = pause;
		thread->Suspend(pause);
	}

protected:
	USocketServerPluginTCPServer* tcpServer = nullptr;
	FClientSocketSession session;
	FRunnableThread* thread = nullptr;
	FSocket* socket = nullptr;
	bool					run = true;
	bool					paused;
	TQueue<FString> messageQueue;
	TQueue<TArray<uint8>> byteArrayQueue;
};


/*Receive Messages or bytes from client. Thread*/
class FTCPClientReceiveDataFromServerThread : public FRunnable {

public:

	FTCPClientReceiveDataFromServerThread(USocketServerPluginTCPServer* tcpServerP,FClientSocketSession sessionP, EReceiveFilterServer receiveFilterP) :
		tcpServer(tcpServerP),
		session(sessionP),
		receiveFilter(receiveFilterP){
		FString threadName = "FTCPClientReceiveDataFromServerThread" + FGuid::NewGuid().ToString();
		thread = FRunnableThread::Create(this, *threadName, 0, EThreadPriority::TPri_Normal);
	}

	virtual uint32 Run() override {
		FString serverID = tcpServer->getServerID();
		FSocket* clientSocket = session.socket;
		FString sessionID = session.sessionID;
				

		//switch to gamethread
		AsyncTask(ENamedThreads::GameThread, [sessionID, serverID]() {
			USocketServerBPLibrary::socketServerBPLibrary->onsocketServerConnectionEventDelegate.Broadcast(EServerSocketConnectionEventType::E_Client, true, "Client connected", sessionID, serverID);
		});

		uint32 DataSize = 0;
		FArrayReaderPtr Datagram = MakeShareable(new FArrayReader(true));
		int64 ticks1;
		int64 ticks2;
		TArray<uint8> byteArray;
		FString recvMessage;
		bool deathConnection = false;
		bool hasData = false;
		while (run && clientSocket != nullptr && tcpServer->isRun()) {
			//ESocketConnectionState::SCS_Connected does not work https://issues.unrealengine.com/issue/UE-27542
			//Compare ticks is a workaround to get a disconnect. clientSocket->Wait() stop working after disconnect. (Another bug?)
			//If it doesn't wait any longer, ticks1 and ticks2 should be the same == disconnect.
			ticks1 = FDateTime::Now().GetTicks();
			clientSocket->Wait(ESocketWaitConditions::WaitForRead, FTimespan::FromSeconds(1));
			ticks2 = FDateTime::Now().GetTicks();


			hasData = false;
			
			if (tcpServer->isRun()) {
				hasData = clientSocket->HasPendingData(DataSize);
			}
			else {
				deathConnection = true;
			}

			if (!hasData && ticks1 == ticks2) {
				//UE_LOG(LogTemp, Display, TEXT("TCP End xxx: %s:%i"), *session.ip, session.port);
				break;
			}
			if (hasData) {

				Datagram->SetNumUninitialized(DataSize);
				int32 BytesRead = 0;
				if (clientSocket->Recv(Datagram->GetData(), Datagram->Num(), BytesRead)) {


					if (receiveFilter == EReceiveFilterServer::E_SAB || receiveFilter == EReceiveFilterServer::E_B) {
						byteArray.Append(Datagram->GetData(), Datagram->Num());
					}


					if (receiveFilter == EReceiveFilterServer::E_SAB || receiveFilter == EReceiveFilterServer::E_S) {
						char* Data = (char*)Datagram->GetData();
						Data[BytesRead] = '\0';
						recvMessage = FString(UTF8_TO_TCHAR(Data));
					}


					//switch to gamethread
					AsyncTask(ENamedThreads::GameThread, [recvMessage, sessionID, byteArray, serverID]() {
						//UE_LOG(LogTemp, Display, TEXT("TCP:%s"), *recvMessage);
						USocketServerBPLibrary::socketServerBPLibrary->onserverReceiveTCPMessageEventDelegate.Broadcast(sessionID, recvMessage, byteArray, serverID);
						if (USocketServerBPLibrary::socketServerBPLibrary->getResiteredClientEvent(sessionID) != nullptr) {
							USocketServerBPLibrary::socketServerBPLibrary->getResiteredClientEvent(sessionID)->onregisteredEventDelegate.Broadcast(recvMessage, byteArray);
						}
					});
				}
				Datagram->Empty();
				byteArray.Empty();
				recvMessage.Empty();

			}
		}
			

		//UE_LOG(LogTemp, Display, TEXT("TCP Connected: %s:%i"), *session.ip, session.port);

		//switch to gamethread
		USocketServerPluginTCPServer* tcpServerGlobal = tcpServer;
		AsyncTask(ENamedThreads::GameThread, [sessionID, serverID, tcpServerGlobal]() {
			USocketServerBPLibrary::socketServerBPLibrary->onsocketServerConnectionEventDelegate.Broadcast(EServerSocketConnectionEventType::E_Client, false, "Client disconnected", sessionID, serverID);
			//clean up socket in main thread because race condition
			tcpServerGlobal->removeClientSession(sessionID);
		});

		thread = nullptr;

		if (run && tcpServer->isRun() == false && deathConnection) {
			return 0;
		}

		if (clientSocket != nullptr) {
			clientSocket->Close();
			clientSocket = nullptr;
			ISocketSubsystem* sSS = USocketServerBPLibrary::socketServerBPLibrary->getSocketSubSystem();
			if (sSS != nullptr) {
				sSS->DestroySocket(clientSocket);
				sSS = nullptr;
			}
		}

		return 0;
	}

	void stopThread() {
		run = false;
	}

protected:
	USocketServerPluginTCPServer* tcpServer = nullptr;
	FClientSocketSession session;
	EReceiveFilterServer receiveFilter;
	FRunnableThread* thread = nullptr;
	bool run = true;
};

/*Receive Files from client. Thread*/
class FTCPClientReceiveFilesFromServerThread : public FRunnable {

public:

	FTCPClientReceiveFilesFromServerThread(USocketServerPluginTCPServer* tcpServerP, FClientSocketSession sessionP, FString downloadDirP) :
		tcpServer(tcpServerP),
		session(sessionP),
		downloadDir(downloadDirP) {
		FString threadName = "FTCPClientReceiveFilesFromServerThread" + FGuid::NewGuid().ToString();
		thread = FRunnableThread::Create(this, *threadName, 0, EThreadPriority::TPri_Normal);
	}

	virtual uint32 Run() override {
		FString serverID = tcpServer->getServerID();
		FSocket* clientSocket = session.socket;
		FString sessionID = session.sessionID;

		//switch to gamethread
		AsyncTask(ENamedThreads::GameThread, [sessionID, serverID]() {
			USocketServerBPLibrary::socketServerBPLibrary->onserverReceiveTCPFileDownloadEventDelegate.Broadcast(1, "Client connected", sessionID, serverID,"");
		});

		if (downloadDir.EndsWith("/") == false && downloadDir.EndsWith("\\") == false) {
			downloadDir = downloadDir + "/";
		}
		FSocketServerDownloadFileInfo fileinfo;
		fileinfo.serverID = serverID;

		FString fullDownloadDir;
		int64 ticksDownload = FDateTime::Now().GetTicks();
		int64 lastByte = 0;
		int64 bytesDownloaded = 0;
		FArchive* writer = nullptr;
		uint32 DataSize;
		FArrayReaderPtr Datagram = MakeShareable(new FArrayReader(true));
		int64 ticks1;
		int64 ticks2;
		TArray<uint8> byteArray;
		FString recvMessage;
		while (run && clientSocket != nullptr && tcpServer->getClientSession(sessionID) != nullptr) {
			//ESocketConnectionState::SCS_Connected does not work https://issues.unrealengine.com/issue/UE-27542
			//Compare ticks is a workaround to get a disconnect. clientSocket->Wait() stop working after disconnect. (Another bug?)
			//If it doesn't wait any longer, ticks1 and ticks2 should be the same == disconnect.
			ticks1 = FDateTime::Now().GetTicks();
			clientSocket->Wait(ESocketWaitConditions::WaitForRead, FTimespan::FromSeconds(5));
			ticks2 = FDateTime::Now().GetTicks();
			bool hasData = clientSocket->HasPendingData(DataSize);
			if (!hasData && ticks1 == ticks2) {
				//UE_LOG(LogTemp, Display, TEXT("TCP End xxx: %s:%i"), *session.ip, session.port);
				break;
			}
			if (hasData) {
				int32 bytesRead = 0;

				switch (downloadStep)
				{
				case 0:
					Datagram->SetNumUninitialized(DataSize);
					if (clientSocket->Recv(Datagram->GetData(), Datagram->Num(), bytesRead)) {
						//client request commands
						//SEND_FILE_TO_SERVER|filename|size|token (size in bytes)
						//RECEIVE_FILE_FROM_SERVER|filename|resumeposition|token (resumeposition in bytes)
						//server answer commands
						//OK|size (in bytes)
						char* Data = (char*)Datagram->GetData();
						Data[bytesRead] = '\0';
						recvMessage = FString(UTF8_TO_TCHAR(Data));
						TArray<FString> lines;
						recvMessage.ParseIntoArray(lines, TEXT("|"), true);


						if (lines.Num() == 3) {
							//client send data to this server
							if (lines[0].Equals("SEND_FILE_TO_SERVER") && lines[1].Len() > 0 && lines[2].Len() > 0) {
								fileinfo.fileName = FPaths::GetCleanFilename(lines[1].TrimStartAndEnd());
								fullDownloadDir = downloadDir + fileinfo.fileName;
								if (FPaths::FileExists(fullDownloadDir)) {
									if (FPlatformFileManager::Get().GetPlatformFile().IsReadOnly(*fullDownloadDir) == false) {
										if (tcpServer->getifFileExistThen() == EHTTPSocketServerFileDownloadResumeType::E_NOT_OVERWRITE) {
											AsyncTask(ENamedThreads::GameThread, [sessionID, serverID, fullDownloadDir]() {
												USocketServerBPLibrary::socketServerBPLibrary->onserverReceiveTCPFileDownloadEventDelegate.Broadcast(-5, "Download aborted because the file exists and the server has been started with 'Cancel Download' ", sessionID, serverID, fullDownloadDir);
												});
											run = false;
											break;
										}
										if (tcpServer->getifFileExistThen() == EHTTPSocketServerFileDownloadResumeType::E_OVERWRITE) {
											FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*fullDownloadDir);
										}
										else {
											writer = IFileManager::Get().CreateFileWriter(*fullDownloadDir, EFileWrite::FILEWRITE_Append);
										}
									}
								}
								else {
									writer = IFileManager::Get().CreateFileWriter(*fullDownloadDir);
								}
								fileSize = FCString::Atoi64(*lines[2]);

								if (!writer) {
									AsyncTask(ENamedThreads::GameThread, [sessionID, serverID, fullDownloadDir]() {
										USocketServerBPLibrary::socketServerBPLibrary->onserverReceiveTCPFileDownloadEventDelegate.Broadcast(-4, "Client can't write into file" , sessionID, serverID,fullDownloadDir);
									});
									run = false;
									break;
								}
								if (tcpServer->getifFileExistThen() == EHTTPSocketServerFileDownloadResumeType::E_RESUME) {
									bytesDownloaded = FPlatformFileManager::Get().GetPlatformFile().FileSize(*fullDownloadDir);
									writer->Seek(bytesDownloaded);
								}

							/*	AsyncTask(ENamedThreads::GameThread, [sessionID, serverID, recvMessage]() {
									USocketServerBPLibrary::socketServerBPLibrary->onserverReceiveTCPFileDownloadEventDelegate.Broadcast(2, "Client file upload to server request accepted. " + recvMessage, sessionID, serverID);
									});*/
								downloadStep++;

								FTCHARToUTF8 Convert(*FString("OK|" + FString::FromInt(bytesDownloaded) + "\r\n"));
								int32 sent = 0;
								clientSocket->Send((uint8*)Convert.Get(), Convert.Length(), sent);
							}
							else {
								//send data to client
								if (lines[0].Equals("RECEIVE_FILE_FROM_SERVER") && lines[1].Len() > 0 && lines[2].Len() > 0) {
									FString dirWithFile = downloadDir + FPaths::GetCleanFilename(lines[1].TrimStartAndEnd());
									fileSize = FCString::Atoi64(*lines[2]);
									if (FPaths::FileExists(dirWithFile)) {

										FTCHARToUTF8 Convert(*FString("OK|" + FString::FromInt(fileSize) + "\r\n"));
										int32 sent = 0;
										clientSocket->Send((uint8*)Convert.Get(), Convert.Length(), sent);

										tcpServer->startTCPDownloadThread(sessionID, dirWithFile, fileSize);

										recvMessage.Empty();
										Datagram->Empty();
										byteArray.Empty();

										if (thread && thread != nullptr) {
											thread = nullptr;
										}
										return 0;
									}
									else {
										AsyncTask(ENamedThreads::GameThread, [sessionID, serverID, dirWithFile]() {
											USocketServerBPLibrary::socketServerBPLibrary->onserverReceiveTCPFileDownloadEventDelegate.Broadcast(-14, "File not found" , sessionID, serverID, dirWithFile);
										});
										run = false;
										break;
									}
								}
								else {
									AsyncTask(ENamedThreads::GameThread, [sessionID, serverID, recvMessage,fullDownloadDir]() {
										USocketServerBPLibrary::socketServerBPLibrary->onserverReceiveTCPFileDownloadEventDelegate.Broadcast(-3, "Client file upload/download request rejected. " + recvMessage, sessionID, serverID,fullDownloadDir);
										});
									run = false;
									break;
								}
								
							}
						}
						else {
							AsyncTask(ENamedThreads::GameThread, [sessionID, serverID, recvMessage,fullDownloadDir]() {
								USocketServerBPLibrary::socketServerBPLibrary->onserverReceiveTCPFileDownloadEventDelegate.Broadcast(-2, "Client file download request rejected. " + recvMessage, sessionID, serverID,fullDownloadDir);
								});
							run = false;
							break;
						}

						fileinfo.size = ((float)fileSize) / 1048576;
						recvMessage.Empty();
						Datagram->Empty();
						byteArray.Empty();

					}
					break;
				case 1:
					//downlnoad
					bytesRead = 0;
					Datagram->Empty();
					Datagram->SetNumUninitialized(DataSize);
					if (clientSocket->Recv(Datagram->GetData(), Datagram->Num(), bytesRead)) {
						writer->Serialize(const_cast<uint8*>(Datagram->GetData()), Datagram->Num());

						//show progress each second
						if ((ticksDownload + 10000000) <= FDateTime::Now().GetTicks()) {
							writer->Flush();
							int64 bytesSendLastSecond = bytesDownloaded - lastByte;
							float speed = ((float)bytesSendLastSecond) / 125000;
							float sent = ((float)bytesDownloaded) / 1048576;
							float left = 0;
							float percent = 0;
							if (fileSize > 0) {
								left = ((float)(fileSize - bytesDownloaded)) / 1048576;
								percent = ((float)bytesDownloaded / (float)fileSize * 100);
							}

							fileinfo.megaBit = speed;
							fileinfo.megaBytesReceived = sent;
							fileinfo.megaBytesLeft = left;
							fileinfo.percentDownload = percent;

							AsyncTask(ENamedThreads::GameThread, [fileinfo]() {
								USocketServerBPLibrary::socketServerBPLibrary->onserverReceiveTCPFileDownloadProgressEventDelegate.Broadcast(fileinfo);
							});

							ticksDownload = FDateTime::Now().GetTicks();
							lastByte = bytesDownloaded;
						}
						bytesDownloaded += bytesRead;
					}
					if (bytesDownloaded >= fileSize) {
						run = false;
					}

				break;

				}
			}
		}

		Datagram->Empty();


		int64 bytesSendLastSecond = bytesDownloaded - lastByte;
		float speed = ((float)bytesSendLastSecond) / 125000;
		float sent = ((float)bytesDownloaded) / 1048576;
		float left = 0;
		float percent = 0;
		if (fileSize > 0) {
			left = ((float)(fileSize - bytesDownloaded)) / 1048576;
			percent = ((float)bytesDownloaded / (float)fileSize * 100);
		}

		fileinfo.megaBit = speed;
		fileinfo.megaBytesReceived = sent;
		fileinfo.megaBytesLeft = left;
		fileinfo.percentDownload = percent;

		AsyncTask(ENamedThreads::GameThread, [fileinfo]() {
			USocketServerBPLibrary::socketServerBPLibrary->onserverReceiveTCPFileDownloadProgressEventDelegate.Broadcast(fileinfo);
		});



		if (FPlatformFileManager::Get().GetPlatformFile().FileSize(*fullDownloadDir) == 0) {
			FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*fullDownloadDir);
		}

		if (writer != nullptr) {
			writer->Close();
			delete writer;
		}

		//USocketServerPluginTCPServer* tcpServerGlobal = tcpServer;
		//AsyncTask(ENamedThreads::GameThread, [sessionID, serverID, tcpServerGlobal]() {
		//	USocketServerBPLibrary::socketServerBPLibrary->onserverReceiveTCPFileDownloadEventDelegate.Broadcast(-1, "Connection will be terminated.", sessionID, serverID);
		//});

		//run = false;


		//UE_LOG(LogTemp, Display, TEXT("TCP Connected: %s:%i"), *session.ip, session.port);


		if (thread && thread != nullptr) {
			thread = nullptr;
		}

		//switch to gamethread
		USocketServerPluginTCPServer* tcpServerGlobal = tcpServer;
		AsyncTask(ENamedThreads::GameThread, [sessionID, serverID, tcpServerGlobal, fileinfo, fullDownloadDir]() {
			if (fileinfo.percentDownload == 100) {
				USocketServerBPLibrary::socketServerBPLibrary->onserverReceiveTCPFileDownloadEventDelegate.Broadcast(2, "File successfully received", sessionID, serverID, fullDownloadDir);
			}
			else {
				USocketServerBPLibrary::socketServerBPLibrary->onserverReceiveTCPFileDownloadEventDelegate.Broadcast(-10, "File Client disconnected", sessionID, serverID, fullDownloadDir);
			}
			
			//clean up socket in main thread because race condition
			tcpServerGlobal->removeClientSession(sessionID);
		});
		//UE_LOG(LogTemp, Display, TEXT("TCP Disconnect: %s:%i"), *session.ip, session.port);

		FPlatformProcess::Sleep(6);
		if (clientSocket != nullptr) {
			clientSocket->Close();
			clientSocket = nullptr;
			ISocketSubsystem* sSS = USocketServerBPLibrary::socketServerBPLibrary->getSocketSubSystem();
			if (sSS != nullptr) {
				sSS->DestroySocket(clientSocket);
			}
		}

		return 0;
	}

	void stopThread() {
		run = false;
	}


protected:
	USocketServerPluginTCPServer* tcpServer = nullptr;
	FClientSocketSession session;
	bool fileServer = false;
	FString downloadDir;
	FRunnableThread* thread = nullptr;
	bool run = true;
	int32 downloadStep = 0;
	int64 fileSize;
};

/*Send Files to client. Thread*/
class FTCPClientSendFileToClientThread : public FRunnable {

public:

	FTCPClientSendFileToClientThread(USocketServerPluginTCPServer* tcpServerP, FClientSocketSession sessionP, FString directoryWithFileNameP, int64 resumeFileSizeP) :
		tcpServer(tcpServerP),
		session(sessionP),
		directoryWithFileName(directoryWithFileNameP),
		resumeFileSize(resumeFileSizeP){
		FString threadName = "FTCPClientSendFileToClientThread" + FGuid::NewGuid().ToString();
		thread = FRunnableThread::Create(this, *threadName, 0, EThreadPriority::TPri_Normal);
	}

	virtual uint32 Run() override {
		FString serverID = tcpServer->getServerID();
		FSocket* clientSocket = session.socket;
		FString sessionID = session.sessionID;
		FString directoryWithFileNameGlobal = directoryWithFileName;

		//switch to gamethread
		AsyncTask(ENamedThreads::GameThread, [sessionID, serverID,directoryWithFileNameGlobal]() {
			USocketServerBPLibrary::socketServerBPLibrary->onserverSendTCPFileUploadEventDelegate.Broadcast(1, "Client connected", sessionID, serverID, directoryWithFileNameGlobal);
		});


		FSocketServerUploadFileInfo fileinfo;
		fileinfo.serverID = serverID;
		

		FArrayReaderPtr Datagram = MakeShareable(new FArrayReader(true));
		TArray<uint8> byteArray;


		FArchive* reader = IFileManager::Get().CreateFileReader(*directoryWithFileName);
		if (!reader) {
			AsyncTask(ENamedThreads::GameThread, [sessionID, serverID, directoryWithFileNameGlobal]() {
				USocketServerBPLibrary::socketServerBPLibrary->onserverSendTCPFileUploadEventDelegate.Broadcast(-5, "Upload aborted because the file cannot be read", sessionID, serverID, directoryWithFileNameGlobal);
			});
			delete reader;
		}
		else {
			TArray<uint8> byteArrayTmp;
			int64 totalSize = reader->TotalSize();
			int64 uploadedBytes = 0;
			int64 ticks = FDateTime::Now().GetTicks();
			int64 ticks1 = 0;
			int64 ticks2 = 0;
			int64 bytesSendLastSecond = 0;
			int64 leftUploadBytes = 1024;

			//resume upload dont start from beginning
			if (resumeFileSize > 0 && resumeFileSize < totalSize) {
				reader->Flush();
				reader->Seek(resumeFileSize);
				uploadedBytes = resumeFileSize;
			}

			if (totalSize < leftUploadBytes)
				leftUploadBytes = totalSize;


			if (resumeFileSize >= totalSize) {
				AsyncTask(ENamedThreads::GameThread, [sessionID, serverID, directoryWithFileNameGlobal]() {
					USocketServerBPLibrary::socketServerBPLibrary->onserverSendTCPFileUploadEventDelegate.Broadcast(-3, "Upload aborted because the start position is equal or higher than the file size " , sessionID, serverID, directoryWithFileNameGlobal);
				});
				uploadedBytes = totalSize;
				run = false;
			}

			fileinfo.fileName = FPaths::GetCleanFilename(directoryWithFileName);
			fileinfo.size = ((float)totalSize) / 1048576;

			int32 disconnectCounter = 10;

			while (run && clientSocket != nullptr && (uploadedBytes + leftUploadBytes) <= totalSize && tcpServer->getClientSession(sessionID) != nullptr) {


				byteArrayTmp.Reset(leftUploadBytes);
				byteArrayTmp.AddUninitialized(leftUploadBytes);
				reader->Serialize(byteArrayTmp.GetData(), byteArrayTmp.Num());
				uploadedBytes += leftUploadBytes;
				bytesSendLastSecond += leftUploadBytes;
				reader->Seek(uploadedBytes);

				int32 sent = 0;
				clientSocket->Send(byteArrayTmp.GetData(), byteArrayTmp.Num(), sent);

				//fail to send data?
				if (sent != byteArrayTmp.Num()) {
					disconnectCounter--;
					if (disconnectCounter <= 0) {
						break;
					}
				}

				//show progress each second
				if ((ticks + 10000000) <= FDateTime::Now().GetTicks()) {
					float speed = ((float)bytesSendLastSecond) / 125000;
					float left = ((float)(totalSize - uploadedBytes)) / 1048576;
					float sentMegabytes = ((float)uploadedBytes) / 1048576;
					float percent = ((float)uploadedBytes / (float)totalSize * 100);
					//UE_LOG(LogTemp, Display, TEXT("FTP Client Upload: left:%f | sent:%f | sentSecond:%f | %f"), left, sent, sentSecond, speed);
					fileinfo.megaBit = speed;
					fileinfo.megaBytesSend = sentMegabytes;
					fileinfo.megaBytesLeft = left;
					fileinfo.percentUpload = percent;

					AsyncTask(ENamedThreads::GameThread, [fileinfo]() {
						USocketServerBPLibrary::socketServerBPLibrary->onserverSendTCPFileUploadProgressEventDelegate.Broadcast(fileinfo);
					});

					ticks = FDateTime::Now().GetTicks();
					bytesSendLastSecond = 0;
				}

				//slow down upload to test
				//FPlatformProcess::Sleep(0.1);
			}

			if (disconnectCounter > 0) {
				leftUploadBytes = totalSize - uploadedBytes;
				if (run && leftUploadBytes > 0 && tcpServer->getClientSession(sessionID) != nullptr) {
					byteArrayTmp.Reset(leftUploadBytes);
					byteArrayTmp.AddUninitialized(leftUploadBytes);
					reader->Serialize(byteArrayTmp.GetData(), byteArrayTmp.Num());
					uploadedBytes += leftUploadBytes;
					bytesSendLastSecond += leftUploadBytes;

					int32 sucessfullUploadedBytes = 0;
					clientSocket->Send(byteArrayTmp.GetData(), byteArrayTmp.Num(), sucessfullUploadedBytes);

					//show progress each second
					float speed = ((float)bytesSendLastSecond) / 125000;
					float left = ((float)(totalSize - uploadedBytes)) / 1048576;
					float sent = ((float)uploadedBytes) / 1048576;
					int32 percent = ((float)uploadedBytes / (float)totalSize * 100);
					//UE_LOG(LogTemp, Display, TEXT("FTP Client Upload: left:%f | sent:%f | sentSecond:%f | %f"), left, sent, sentSecond, speed);
					fileinfo.megaBit = speed;
					fileinfo.megaBytesSend = sent;
					fileinfo.megaBytesLeft = left;
					fileinfo.percentUpload = percent;

					AsyncTask(ENamedThreads::GameThread, [fileinfo]() {
						USocketServerBPLibrary::socketServerBPLibrary->onserverSendTCPFileUploadProgressEventDelegate.Broadcast(fileinfo);
						});

				}
				else {
					//show progress each second
					float speed = ((float)bytesSendLastSecond) / 125000;
					float left = ((float)(totalSize - uploadedBytes)) / 1048576;
					float sent = ((float)uploadedBytes) / 1048576;
					int32 percent = ((float)uploadedBytes / (float)totalSize * 100);
					//UE_LOG(LogTemp, Display, TEXT("FTP Client Upload: left:%f | sent:%f | sentSecond:%f | %f"), left, sent, sentSecond, speed);
					fileinfo.megaBit = speed;
					fileinfo.megaBytesSend = sent;
					fileinfo.megaBytesLeft = left;
					fileinfo.percentUpload = percent;

					AsyncTask(ENamedThreads::GameThread, [fileinfo]() {
						USocketServerBPLibrary::socketServerBPLibrary->onserverSendTCPFileUploadProgressEventDelegate.Broadcast(fileinfo);
					});
				}
			}

		}



		Datagram->Empty();
		byteArray.Empty();



		if (reader != nullptr) {
			reader->Close();
			delete reader;
		}



		if (thread && thread != nullptr) {
			thread = nullptr;
		}

		//switch to gamethread
		USocketServerPluginTCPServer* tcpServerGlobal = tcpServer;
		AsyncTask(ENamedThreads::GameThread, [sessionID, serverID, tcpServerGlobal, fileinfo, directoryWithFileNameGlobal]() {
			if (fileinfo.percentUpload == 100) {
				USocketServerBPLibrary::socketServerBPLibrary->onserverReceiveTCPFileDownloadEventDelegate.Broadcast(2, "File successfully sent", sessionID, serverID, directoryWithFileNameGlobal);
			}
			else {
				USocketServerBPLibrary::socketServerBPLibrary->onserverReceiveTCPFileDownloadEventDelegate.Broadcast(-10, "File Upload Client disconnected", sessionID, serverID, directoryWithFileNameGlobal);
			}
			
			//clean up socket in main thread because race condition
			tcpServerGlobal->removeClientSession(sessionID);
		});
		//UE_LOG(LogTemp, Display, TEXT("TCP Disconnect: %s:%i"), *session.ip, session.port);

		FPlatformProcess::Sleep(6);
		if (clientSocket != nullptr) {
			clientSocket->Close();
			clientSocket = nullptr;
			ISocketSubsystem* sSS = USocketServerBPLibrary::socketServerBPLibrary->getSocketSubSystem();
			if (sSS != nullptr) {
				sSS->DestroySocket(clientSocket);
			}
		}

		return 0;
	}

	void stopThread() {
		run = false;
	}


protected:
	USocketServerPluginTCPServer* tcpServer = nullptr;
	FClientSocketSession session;
	bool fileServer = false;
	FString directoryWithFileName;
	FRunnableThread* thread = nullptr;
	bool run = true;
	int32 downloadStep = 0;
	int64 fileSize;
	int64 resumeFileSize = 0;
};

