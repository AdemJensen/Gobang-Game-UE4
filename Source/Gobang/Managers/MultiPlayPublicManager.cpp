// This is a private project, cannot be put at commercial use.


#include "MultiPlayPublicManager.h"

AMultiPlayPublicManager* AMultiPlayPublicManager::InitMultiPlayPublicManagerByValue(EBanMode BanModeValue, float HintCoolDownValue, int32 RetractTimesValue, float RoundTimeLimitValue, FString Ip, int32 Port)
{
	SetBanMode(BanModeValue);
	SetHintCoolDownTime(HintCoolDownValue);
	SetTotalRetractTimes(RetractTimesValue);
	SetRoundTimeLimit(RoundTimeLimitValue);
	SetConnectionIp(Ip);
	SetConnectionPort(Port);
	return this;
}


void AMultiPlayPublicManager::OnReceiveMessage(FString msg)
{
}