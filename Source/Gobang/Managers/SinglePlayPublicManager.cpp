// This is a private project, cannot be put at commercial use.


#include "SinglePlayPublicManager.h"

void ASinglePlayPublicManager::InitSinglePlayPublicManager(int32 DifficultyLevel, int32 BanModeLevel, int32 RetractTimesLevel, int32 RoundTimeLimitLevel, int32 HintCoolDownLevel)
{
	SetAiDifficulty(DifficultyLevel + 1);
	SetBanMode((EBanMode) (BanModeLevel + 1));
	switch (HintCoolDownLevel)
	{
	case 0:
		SetHintCoolDownTime(0.0f);
		break;
	case 1:
		SetHintCoolDownTime(15.0f);
		break;
	case 2:
		SetHintCoolDownTime(30.0f);
		break;
	case 3:
		SetHintCoolDownTime(45.0f);
		break;
	case 4:
		SetHintCoolDownTime(60.0f);
		break;
	default:
		SetHintCoolDownTime(-1.0f);
	}
	switch (RetractTimesLevel)
	{
	case 0:
		SetTotalRetractTimes(0);
		break;
	case 1:
		SetTotalRetractTimes(1);
		break;
	case 2:
		SetTotalRetractTimes(3);
		break;
	case 3:
		SetTotalRetractTimes(5);
		break;
	case 4:
		SetTotalRetractTimes(10);
		break;
	default:
		SetTotalRetractTimes(-1);
	}
	switch (RoundTimeLimitLevel)
	{
	case 0:
		SetRoundTimeLimit(15.0f);
		break;
	case 1:
		SetRoundTimeLimit(30.0f);
		break;
	case 2:
		SetRoundTimeLimit(45.0f);
		break;
	case 3:
		SetRoundTimeLimit(60.0f);
		break;
	default:
		SetRoundTimeLimit(0.0f);
	}
}
