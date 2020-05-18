// Copyright 2017-2020 David Romanski (Socke). All Rights Reserved.

#include "DNSClientSocketServer.h"

UDNSClientSocketServer::UDNSClientSocketServer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {

	onresolveDomainEventDelegate.AddDynamic(this, &UDNSClientSocketServer::resolveDomainEventDelegate);
}

void UDNSClientSocketServer::resolveDomainEventDelegate(const FString IP) {}


void UDNSClientSocketServer::resolveDomain(ISocketSubsystem * socketSubSystem, FString domainP, bool useDNSCache, FString dnsIP ) {
	domain = domainP;
	resolving = true;
	if (useDNSCache) {
		if (dnsCache.Find(domain) != nullptr) {
			ip = *dnsCache.Find(domain);
			resolving = false;
			return;
		}
	}
	FIPv4Endpoint Endpoint;
	FString socketName;
	//FIPv4Endpoint::Parse("127.0.0.1:12345", Endpoint);
	//socket = FUdpSocketBuilder(*socketName).AsReusable().WithBroadcast().BoundToEndpoint(Endpoint);
	socket = FUdpSocketBuilder(*socketName).AsReusable().WithBroadcast();
	if (socket == nullptr || socket == NULL) {
		UE_LOG(LogTemp, Error, TEXT("UE4 could not init a UDP socket to resolve %s on %s."), *domain, *dnsIP);
		resolving = false;
		return;
	}
	else {
		FTimespan ThreadWaitTime = FTimespan::FromMilliseconds(100);
		FUdpSocketReceiver* udpSocketReceiver = new FUdpSocketReceiver(socket, ThreadWaitTime, TEXT("UE4 DNSClient"));
		udpSocketReceiver->OnDataReceived().BindUObject(this, &UDNSClientSocketServer::UDPReceiver);
		udpSocketReceiver->Start();

		TSharedRef<FInternetAddr> addr = socketSubSystem->CreateInternetAddr();
		bool bIsValid;
		addr->SetIp(*dnsIP, bIsValid);
		addr->SetPort(53);
		if (bIsValid) {
			TArray<unsigned char> dnsRequest;
			//id (ramdom choose)
			dnsRequest.Add(0x22);
			dnsRequest.Add(0x76);
			//head
			dnsRequest.Add(0x1);
			dnsRequest.Add(0x0);
			dnsRequest.Add(0x0);
			dnsRequest.Add(0x1);
			dnsRequest.Add(0x0);
			dnsRequest.Add(0x0);
			dnsRequest.Add(0x0);
			dnsRequest.Add(0x0);
			dnsRequest.Add(0x0);
			dnsRequest.Add(0x0);

			//domain
			//split in domain parts than add the length as byte and the part as bytes
			TArray<FString> domainParts;
			domain.ParseIntoArray(domainParts, TEXT("."), true);
			for (int i = 0; i < domainParts.Num(); i++) {
				TCHAR *serializedChar = domainParts[i].GetCharArray().GetData();
				ANSICHAR* utf8text = TCHAR_TO_UTF8(serializedChar);
				int32 size = FString(utf8text).Len();
				dnsRequest.Add(domainParts[i].Len());
				dnsRequest.Append((uint8*)utf8text,size);
			}
			//foot
			dnsRequest.Add(0x0);
			dnsRequest.Add(0x0);
			dnsRequest.Add(0x1);
			dnsRequest.Add(0x0);
			dnsRequest.Add(0x1);
			int32 sent;
			socket->SendTo((uint8*)dnsRequest.GetData(), dnsRequest.Num(), sent, *addr);
		}
	}

}

void UDNSClientSocketServer::UDPReceiver(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt) {

	//TSharedPtr<FInternetAddr> peerAddr = EndPt.ToInternetAddr();
	//FString ip = peerAddr->ToString(false);
	//int32 port = peerAddr->GetPort();
	////UE_LOG(LogTemp, Error, TEXT("peerAddr:%s  port:%i"), *peerAddr->ToString(false), peerAddr->GetPort());



	TArray<uint8> byteArray;
	byteArray.Append(ArrayReaderPtr->GetData(), ArrayReaderPtr->Num());

	FString ipAdress;
	if (byteArray.Num() > 10) {
		for (int32 i = (byteArray.Num() - 4); i < byteArray.Num(); i++) {
			uint32 ipTmp = byteArray.GetData()[i] << 0;
			ipAdress += FString::FromInt(ipTmp)+".";
		}
	}

	ipAdress.RemoveFromEnd(".");

	//UE_LOG(LogTemp, Display, TEXT("DNS Resolved IP:%s"), *recvMessage);

	ip = ipAdress;
	dnsCache.Add(domain, ip);

	resolving = false;

	UDNSClientSocketServer* self = this;

	AsyncTask(ENamedThreads::GameThread, [ipAdress,self]() {
		self->onresolveDomainEventDelegate.Broadcast(ipAdress);
	});
}

bool UDNSClientSocketServer::isResloving(){
	return resolving;
}

FString UDNSClientSocketServer::getIP(){
	return ip;
}
