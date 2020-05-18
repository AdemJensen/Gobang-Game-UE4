// Copyright 2018-2020 David Romanski (Socke). All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "EventBean.generated.h"


/**
*
*/
UCLASS(Blueprintable, BlueprintType)
class UEventBean : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FregisteredEventDelegate, const FString, message, const TArray<uint8>&, byteArray);
	UFUNCTION()
		void registeredEventDelegate(const FString message, const TArray<uint8>& byteArray);
	UPROPERTY(BlueprintAssignable, Category = "SocketServer|Register|Events")
		FregisteredEventDelegate onregisteredEventDelegate;

private:

};
