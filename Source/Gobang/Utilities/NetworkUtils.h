// This is a private project, cannot be put at commercial use.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "json.hpp"
#include "NetworkUtils.generated.h"

using Json = nlohmann::json;

/**
 * 
 */
UCLASS()
class GOBANG_API UNetworkUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "Network Utils")
		static FString GetHost(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "Network Utils")
		static int32 GetPortNumber(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Network Utils")
		static FString DecodeNetworkMessage(FString str);
	UFUNCTION(BlueprintCallable, Category = "Network Utils")
		static FString EncodeNetworkMessage(FString str);
	
};
