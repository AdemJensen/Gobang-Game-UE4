// This is a private project, cannot be put at commercial use.


#include "BlinkHelper.h"
#include "Engine.h"

// Sets default values for this component's properties
ABlinkHelper::ABlinkHelper()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryActorTick.bCanEverTick = true;

	// ...
}

void ABlinkHelper::StartBlink(float GapTime, float SustainTime, int32 RepeteTimes, bool bDoLastHidden)
{
	if (RepeteTimes == 0) return;
	Gap = GapTime;
	Sustain = SustainTime;
	RepeteRemain = RepeteTimes;
	bDoLast = bDoLastHidden;
	ShowSignDelegate.Broadcast();
	BP_ShowSignDelegate.Broadcast();
	bInSustain = true;
	StartDowncount(Sustain, 0.1);
}

void ABlinkHelper::OnTimeUp()
{
	if (bInSustain)
	{
		bInSustain = false;
		if (RepeteRemain > 1 || bDoLast) HideSignDelegate.Broadcast();
		BP_HideSignDelegate.Broadcast();
		StartDowncount(Gap - Sustain, 0.1);
	}
	else
	{
		RepeteRemain--;
		if (RepeteRemain > 0)
		{
			bInSustain = true;
			ShowSignDelegate.Broadcast();
			BP_ShowSignDelegate.Broadcast();
			StartDowncount(Sustain, 0.1);
		}
	}
}

