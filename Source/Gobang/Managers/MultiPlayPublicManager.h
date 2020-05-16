// This is a private project, cannot be put at commercial use.

#pragma once

#include "CoreMinimal.h"
#include "PublicManagerBase.h"
#include "../Basics/GameStage.h"
#include "../Basics/ChessType.h"
#include "../Basics/BanMode.h"
#include "MultiPlayPublicManager.generated.h"

/**
 * 
 */
UCLASS()
class GOBANG_API AMultiPlayPublicManager : public APublicManagerBase
{
	GENERATED_BODY()

public:

	virtual EPublicManagerType GetManagerType() { return EPublicManagerType::MULTI_PLAY; }

	UFUNCTION(BlueprintCallable, Category = "Game Info")
		AMultiPlayPublicManager* InitMultiPlayPublicManagerByValue(EBanMode BanModeValue, float HintCoolDownValue, int32 RetractTimesValue, float RoundTimeLimitValue, FString Ip, int32 Port);

	UFUNCTION(BlueprintCallable, Category = "Connection Info")
		void SetConnectionIp(FString Ip) { ConnectionIp = Ip; }
	UFUNCTION(BlueprintCallable, Category = "Connection Info")
		FString GetConnectionIp() { return ConnectionIp; }

	UFUNCTION(BlueprintCallable, Category = "Connection Info")
		void SetConnectionPort(int32 Port) { ConnectionPort = Port; }
	UFUNCTION(BlueprintCallable, Category = "Connection Info")
		int32 GetConnectionPort() { return ConnectionPort; }

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connection Info")
		bool IsServer = false;		// Server port, valid when SERVER and CLIENT.

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connection Info")
		FString ConnectionIp = "127.0.0.1";		// This is valid when current is CLIENT.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connection Info")
		int32 ConnectionPort = 0;	// Server port, valid when SERVER and CLIENT.

	UFUNCTION(BlueprintCallable, Category = "Network Events")
		void OnReceiveMessage(FString Msg);

};
