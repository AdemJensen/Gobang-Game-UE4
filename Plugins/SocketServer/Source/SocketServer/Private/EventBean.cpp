// Copyright 2018-2020 David Romanski (Socke). All Rights Reserved.

#include "EventBean.h"


UEventBean::UEventBean(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

}

//void UEventBean::JavascriptEventTag(const FString ID, const FString ClassName, const FString Value, const TArray<FString>& args) {}

void UEventBean::registeredEventDelegate(const FString message, const TArray<uint8>& byteArray)
{
}
