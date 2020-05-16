// This is a private project, cannot be put at commercial use.

#include "SinglePlayPublicManager.h"

ASinglePlayPublicManager* ASinglePlayPublicManager::InitSinglePlayPublicManagerByValue(int32 DifficultyValue, EBanMode BanModeValue, float HintCoolDownValue, int32 RetractTimesValue, float RoundTimeLimitValue)
{
	SetAiDifficulty(DifficultyValue);
	SetBanMode(BanModeValue);
	SetHintCoolDownTime(HintCoolDownValue);
	SetTotalRetractTimes(RetractTimesValue);
	SetRoundTimeLimit(RoundTimeLimitValue);
	return this;
}
