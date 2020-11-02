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

TArray<FString> UNetworkUtils::DecodeNetworkMessage(FString str)
{
    int i = 0, j;
    TArray<FString> Temp;
    for (j = 0; j < str.Len() - 1 && i < str.Len(); j++) {
        if (str[j] == '|' && str[j + 1] == '|') {
            FString var = str.LeftChop(str.Len() - j - 1).RightChop(i);
            UE_LOG(LogTemp, Warning, TEXT("%s"), *var);
            Temp.Add(var);
            j++;
            i = j + 1;
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("OK aft split"));
    for (int k = 0; k < Temp.Num(); k++) {
        FString TmpStr = Temp[k];
        UE_LOG(LogTemp, Warning, TEXT("Str to chop: %s"), *TmpStr);
        for (i = 0; i < TmpStr.Len(); i++) {
            if (TmpStr[i] == '{') break;
        }
        for (j = TmpStr.Len() - 1; j >= 0; j--) {
            if (TmpStr[j] == '}') break;
        }
        Temp[k] = TmpStr.LeftChop(TmpStr.Len() - j - 1).RightChop(i);
        UE_LOG(LogTemp, Warning, TEXT("Str chopped: %s"), *Temp[k]);
    }
    
    return Temp;
}
FString UNetworkUtils::EncodeNetworkMessage(FString str)
{
    return "==========" + str + "==========||";
}