// Copyright 2017-2020 David Romanski (Socke). All Rights Reserved.

#include "FileFunctionsSocketServer.h"


UFileFunctionsSocketServer::UFileFunctionsSocketServer(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

}

FString UFileFunctionsSocketServer::getCleanDirectory(EFileFunctionsSocketServerDirectoryType directoryType, FString filePath) {
	if (directoryType == EFileFunctionsSocketServerDirectoryType::E_ad) {
		return FPaths::ConvertRelativePathToFull(filePath);
	}
	else {
		FString ProjectDir = FPaths::ProjectDir();
		return FPaths::ConvertRelativePathToFull(ProjectDir + filePath);
	}
}

void UFileFunctionsSocketServer::writeBytesToFile(EFileFunctionsSocketServerDirectoryType directoryType, FString filePath, TArray<uint8> bytes, bool& success) {
	success = FFileHelper::SaveArrayToFile(bytes, *getCleanDirectory(directoryType, filePath));
}

void UFileFunctionsSocketServer::addBytesToFile(EFileFunctionsSocketServerDirectoryType directoryType, FString filePath, TArray<uint8> bytes, bool& success) {
	FArchive* writer = IFileManager::Get().CreateFileWriter(*getCleanDirectory(directoryType, filePath), EFileWrite::FILEWRITE_Append);
	if (!writer) {
		success = false;
		return;
	}
	writer->Seek(writer->TotalSize());
	writer->Serialize(bytes.GetData(), bytes.Num());
	writer->Close();
	delete writer;

	success = true;
}

//void UFileFunctionsSocketServer::splittFile(EFileFunctionsSocketServerDirectoryType directoryType, FString filePath, int32 parts, bool& success){
//	if (parts <= 0)
//		parts = 1;
//	FArchive* reader = IFileManager::Get().CreateFileReader(*getCleanDirectory(directoryType, filePath));
//	if (!reader) {
//		success = false;
//		return;
//	}
//	
//	int64 splittAfterBytes = reader->TotalSize()/ ((int64)parts);
//	TArray<uint8> bytes;
//
//	for (int32 i = 0; i < parts; i++){
//		bytes.AddUninitialized(splittAfterBytes);
//		reader->Serialize(bytes.GetData(), splittAfterBytes);
//		if (FFileHelper::SaveArrayToFile(bytes, *getCleanDirectory(directoryType, filePath)) == false) {
//			success = false;
//			return;
//		}
//		splittAfterBytes =
//		reader->Seek();
//	}
//
//}

TArray<uint8> UFileFunctionsSocketServer::readBytesFromFile(EFileFunctionsSocketServerDirectoryType directoryType, FString filePath, bool& success) {
	TArray<uint8> result;
	success = FFileHelper::LoadFileToArray(result, *getCleanDirectory(directoryType, filePath));
	return result;
}

void UFileFunctionsSocketServer::readStringFromFile(EFileFunctionsSocketServerDirectoryType directoryType, FString filePath, bool& success, FString& data) {
	success = FFileHelper::LoadFileToString(data, *getCleanDirectory(directoryType, filePath));
}

void UFileFunctionsSocketServer::writeStringToFile(EFileFunctionsSocketServerDirectoryType directoryType, FString data, FString filePath, bool& success) {
	success = FFileHelper::SaveStringToFile(data, *getCleanDirectory(directoryType, filePath));
}


void UFileFunctionsSocketServer::getMD5FromFile(EFileFunctionsSocketServerDirectoryType directoryType, FString filePath, bool& success, FString& MD5) {
	FArchive* reader = IFileManager::Get().CreateFileReader(*getCleanDirectory(directoryType, filePath));
	if (!reader) {
		success = false;
		return;
	}

	TArray<uint8> byteArrayTmp;
	int64 totalSize = reader->TotalSize();
	int64 loadedBytes = 0;
	int64 leftUploadBytes = 1024;


	if (totalSize < leftUploadBytes)
		leftUploadBytes = totalSize;


	uint8 Digest[16];
	FMD5 Md5Gen;

	while ((loadedBytes + leftUploadBytes) <= totalSize) {
		byteArrayTmp.Reset(leftUploadBytes);
		byteArrayTmp.AddUninitialized(leftUploadBytes);
		reader->Serialize(byteArrayTmp.GetData(), byteArrayTmp.Num());
		loadedBytes += leftUploadBytes;
		reader->Seek(loadedBytes);

		Md5Gen.Update(byteArrayTmp.GetData(), byteArrayTmp.Num());
	}

	leftUploadBytes = totalSize - loadedBytes;
	if (leftUploadBytes > 0) {
		byteArrayTmp.Reset(leftUploadBytes);
		byteArrayTmp.AddUninitialized(leftUploadBytes);
		reader->Serialize(byteArrayTmp.GetData(), byteArrayTmp.Num());
		loadedBytes += leftUploadBytes;
		Md5Gen.Update(byteArrayTmp.GetData(), byteArrayTmp.Num());
	}

	if (reader != nullptr) {
		reader->Close();
		delete reader;
	}

	if (totalSize != loadedBytes) {
		success = false;
		return;
	}

	Md5Gen.Final(Digest);
	for (int32 i = 0; i < 16; i++) {
		MD5 += FString::Printf(TEXT("%02x"), Digest[i]);
	}

	success = true;
}


void UFileFunctionsSocketServer::bytesToBase64String(TArray<uint8> bytes, FString& base64String) {
	base64String = FBase64::Encode(bytes);
}

TArray<uint8> UFileFunctionsSocketServer::base64StringToBytes(EFileFunctionsSocketServerDirectoryType directoryType, FString base64String, bool& success) {
	TArray<uint8> fileData;
	success = FBase64::Decode(*base64String, fileData);
	return fileData;
}

void UFileFunctionsSocketServer::fileToBase64String(EFileFunctionsSocketServerDirectoryType directoryType, FString filePath, bool& success, FString& base64String, FString& fileName) {
	TArray<uint8> fileData;
	if (!FFileHelper::LoadFileToArray(fileData, *getCleanDirectory(directoryType, filePath))) {
		success = false;
		return;
	}
	base64String = FBase64::Encode(fileData);
	success = true;
}

bool UFileFunctionsSocketServer::fileExists(EFileFunctionsSocketServerDirectoryType directoryType, FString filePath) {
	return FPaths::FileExists(*getCleanDirectory(directoryType, filePath));
}

bool UFileFunctionsSocketServer::directoryExists(EFileFunctionsSocketServerDirectoryType directoryType, FString path) {
	return FPaths::DirectoryExists(*getCleanDirectory(directoryType, path));
}

int64 UFileFunctionsSocketServer::fileSize(EFileFunctionsSocketServerDirectoryType directoryType, FString filePath) {
	return FPlatformFileManager::Get().GetPlatformFile().FileSize(*getCleanDirectory(directoryType, filePath));
}

bool UFileFunctionsSocketServer::deleteFile(EFileFunctionsSocketServerDirectoryType directoryType, FString filePath) {
	return FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*getCleanDirectory(directoryType, filePath));
}

bool UFileFunctionsSocketServer::deleteDirectory(EFileFunctionsSocketServerDirectoryType directoryType, FString filePath) {
	return FPlatformFileManager::Get().GetPlatformFile().DeleteDirectory(*getCleanDirectory(directoryType, filePath));
}

bool UFileFunctionsSocketServer::isReadOnly(EFileFunctionsSocketServerDirectoryType directoryType, FString filePath) {
	return FPlatformFileManager::Get().GetPlatformFile().IsReadOnly(*getCleanDirectory(directoryType, filePath));
}

bool UFileFunctionsSocketServer::moveFile(EFileFunctionsSocketServerDirectoryType directoryTypeTo, FString filePathTo, EFileFunctionsSocketServerDirectoryType directoryTypeFrom, FString filePathFrom) {
	return FPlatformFileManager::Get().GetPlatformFile().MoveFile(*getCleanDirectory(directoryTypeTo, filePathTo), *getCleanDirectory(directoryTypeFrom, filePathFrom));
}

bool UFileFunctionsSocketServer::setReadOnly(EFileFunctionsSocketServerDirectoryType directoryType, FString filePath, bool bNewReadOnlyValue) {
	return FPlatformFileManager::Get().GetPlatformFile().SetReadOnly(*getCleanDirectory(directoryType, filePath), bNewReadOnlyValue);
}

FDateTime UFileFunctionsSocketServer::getTimeStamp(EFileFunctionsSocketServerDirectoryType directoryType, FString filePath) {
	return FPlatformFileManager::Get().GetPlatformFile().GetTimeStamp(*getCleanDirectory(directoryType, filePath));
}

void UFileFunctionsSocketServer::setTimeStamp(EFileFunctionsSocketServerDirectoryType directoryType, FString filePath, FDateTime DateTime) {
	FPlatformFileManager::Get().GetPlatformFile().SetTimeStamp(*getCleanDirectory(directoryType, filePath), DateTime);
}

FDateTime UFileFunctionsSocketServer::getAccessTimeStamp(EFileFunctionsSocketServerDirectoryType directoryType, FString filePath) {
	return FPlatformFileManager::Get().GetPlatformFile().GetAccessTimeStamp(*getCleanDirectory(directoryType, filePath));
}

FString UFileFunctionsSocketServer::getFilenameOnDisk(EFileFunctionsSocketServerDirectoryType directoryType, FString filePath) {
	return FPlatformFileManager::Get().GetPlatformFile().GetFilenameOnDisk(*getCleanDirectory(directoryType, filePath));
}

bool UFileFunctionsSocketServer::createDirectory(EFileFunctionsSocketServerDirectoryType directoryType, FString path) {
	return FPlatformFileManager::Get().GetPlatformFile().CreateDirectory(*getCleanDirectory(directoryType, path));
}