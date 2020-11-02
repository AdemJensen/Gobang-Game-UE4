// Copyright 2017-2020 David Romanski (Socke). All Rights Reserved.
#pragma once

#include "SocketServer.h"
#include "FileFunctionsSocketServer.generated.h"



UCLASS(Blueprintable, BlueprintType)
class UFileFunctionsSocketServer : public UObject
{
	GENERATED_UCLASS_BODY()

public:


	//APEND bytes in eine Datei muss vorhenden sein fuer webcom plugin. vielleicht auch eine node zum aufteilen einer datei

	static FString getCleanDirectory(EFileFunctionsSocketServerDirectoryType directoryType, FString filePath);

	UFUNCTION(BlueprintCallable, Category = "SocketServer|SpecialFunctions|File")
		static void writeBytesToFile(EFileFunctionsSocketServerDirectoryType directoryType, FString filePath, TArray<uint8> bytes, bool& success);
	UFUNCTION(BlueprintCallable, Category = "SocketServer|SpecialFunctions|File")
		static void addBytesToFile(EFileFunctionsSocketServerDirectoryType directoryType, FString filePath, TArray<uint8> bytes, bool& success);
	//UFUNCTION(BlueprintCallable, Category = "SocketServer|SpecialFunctions|File")
	//	static void splittFile(EFileFunctionsSocketServerDirectoryType directoryType, FString filePath, int32 parts, bool& success);
	UFUNCTION(BlueprintCallable, Category = "SocketServer|SpecialFunctions|File")
		static TArray<uint8> readBytesFromFile(EFileFunctionsSocketServerDirectoryType directoryType, FString filePath, bool &success);
	UFUNCTION(BlueprintCallable, Category = "SocketServer|SpecialFunctions|File")
		static void readStringFromFile(EFileFunctionsSocketServerDirectoryType directoryType, FString filePath, bool& success, FString& data);
	UFUNCTION(BlueprintCallable, Category = "SocketServer|SpecialFunctions|File")
		static void writeStringToFile(EFileFunctionsSocketServerDirectoryType directoryType, FString data, FString filePath, bool& success);

	UFUNCTION(BlueprintCallable, Category = "SocketServer|SpecialFunctions|File")
		static void getMD5FromFile(EFileFunctionsSocketServerDirectoryType directoryType, FString filePath, bool& success, FString& MD5);
	UFUNCTION(BlueprintCallable, Category = "SocketServer|SpecialFunctions|File")
		static void bytesToBase64String(TArray<uint8> bytes, FString& base64String);
	UFUNCTION(BlueprintCallable, Category = "SocketServer|SpecialFunctions|File")
		static TArray<uint8> base64StringToBytes(EFileFunctionsSocketServerDirectoryType directoryType, FString base64String, bool& success);
	UFUNCTION(BlueprintCallable, Category = "SocketServer|SpecialFunctions|File")
		static void fileToBase64String(EFileFunctionsSocketServerDirectoryType directoryType, FString filePath, bool& success, FString& base64String, FString& fileName);
	UFUNCTION(BlueprintCallable, Category = "SocketServer|SpecialFunctions|File")
		static bool fileExists(EFileFunctionsSocketServerDirectoryType directoryType, FString filePath);
	UFUNCTION(BlueprintCallable, Category = "SocketServer|SpecialFunctions|File")
		static bool directoryExists(EFileFunctionsSocketServerDirectoryType directoryType, FString path);
	UFUNCTION(BlueprintCallable, Category = "SocketServer|SpecialFunctions|File")
		static int64 fileSize(EFileFunctionsSocketServerDirectoryType directoryType, FString filePath);
	UFUNCTION(BlueprintCallable, Category = "SocketServer|SpecialFunctions|File")
		static bool deleteFile(EFileFunctionsSocketServerDirectoryType directoryType, FString filePath);
	/** Delete a directory and return true if the directory was deleted or otherwise does not exist. **/
	UFUNCTION(BlueprintCallable, Category = "SocketServer|SpecialFunctions|File")
		static bool deleteDirectory(EFileFunctionsSocketServerDirectoryType directoryType, FString filePath);
	/** Return true if the file is read only. **/
	UFUNCTION(BlueprintCallable, Category = "SocketServer|SpecialFunctions|File")
		static bool isReadOnly(EFileFunctionsSocketServerDirectoryType directoryType, FString filePath);
	/** Attempt to move a file. Return true if successful. Will not overwrite existing files. **/
	UFUNCTION(BlueprintCallable, Category = "SocketServer|SpecialFunctions|File")
		static bool moveFile(EFileFunctionsSocketServerDirectoryType directoryTypeTo, FString filePathTo, EFileFunctionsSocketServerDirectoryType directoryTypeFrom, FString filePathFrom);
	/** Attempt to change the read only status of a file. Return true if successful. **/
	UFUNCTION(BlueprintCallable, Category = "SocketServer|SpecialFunctions|File")
		static bool setReadOnly(EFileFunctionsSocketServerDirectoryType directoryType, FString filePath, bool bNewReadOnlyValue);
	/** Return the modification time of a file. Returns FDateTime::MinValue() on failure **/
	UFUNCTION(BlueprintCallable, Category = "SocketServer|SpecialFunctions|File")
		static FDateTime getTimeStamp(EFileFunctionsSocketServerDirectoryType directoryType, FString filePath);
	/** Sets the modification time of a file **/
	UFUNCTION(BlueprintCallable, Category = "SocketServer|SpecialFunctions|File")
		static void	setTimeStamp(EFileFunctionsSocketServerDirectoryType directoryType, FString filePath, FDateTime DateTime);
	/** Return the last access time of a file. Returns FDateTime::MinValue() on failure **/
	UFUNCTION(BlueprintCallable, Category = "SocketServer|SpecialFunctions|File")
		static FDateTime getAccessTimeStamp(EFileFunctionsSocketServerDirectoryType directoryType, FString filePath);
	/** For case insensitive filesystems, returns the full path of the file with the same case as in the filesystem */
	UFUNCTION(BlueprintCallable, Category = "SocketServer|SpecialFunctions|File")
		static FString getFilenameOnDisk(EFileFunctionsSocketServerDirectoryType directoryType, FString filePath);
	/** Create a directory and return true if the directory was created or already existed. **/
	UFUNCTION(BlueprintCallable, Category = "SocketServer|SpecialFunctions|File")
		static bool createDirectory(EFileFunctionsSocketServerDirectoryType directoryType, FString path);

private:


};

