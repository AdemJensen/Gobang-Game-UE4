// This is a private project, cannot be put at commercial use.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NetworkUtils.generated.h"

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
	
};
