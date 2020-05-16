// This is a private project, cannot be put at commercial use.


#include "NetworkUtils.h"
#include "Engine.h"
#include "SocketSubsystem.h"

FString UNetworkUtils::GetHost(UObject* WorldContextObject)
{
    if (WorldContextObject)
    {
        if (UWorld* World = WorldContextObject->GetWorld())
        {
            bool canBind = false;
            TSharedRef<FInternetAddr> localIp = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, canBind);
            return (localIp->IsValid() ? localIp->ToString(false) : World->URL.Host);
            return World->URL.Host;
        }
    }
    return "Not Avail";
}


int32 UNetworkUtils::GetPortNumber(UObject* WorldContextObject)
{
    if (WorldContextObject)
    {
        if (UWorld* World = WorldContextObject->GetWorld())
        {
            return World->URL.Port;
        }
    }
    return -1;
}

FString UNetworkUtils::DecodeNetworkMessage(FString str)
{
    int i, j;
    for (i = 0; i < str.Len(); i++) {
        if (str[i] == '{') break;
    }
    for (j = str.Len() - 1; j >= 0; j--) {
        if (str[j] == '}') break;
    }
    return str.LeftChop(str.Len() - j - 1).RightChop(i);
}
FString UNetworkUtils::EncodeNetworkMessage(FString str)
{
    return "==========" + str + "==========";
}