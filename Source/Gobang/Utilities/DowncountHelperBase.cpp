// This is a private project, cannot be put at commercial use.


#include "DowncountHelperBase.h"
#include "Engine.h"

// Sets default values for this component's properties
ADowncountHelperBase::ADowncountHelperBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryActorTick.bCanEverTick = true;

	// ...
	bReplicates = false;
}


void ADowncountHelperBase::StartDowncount(float TotalTime, float UnitTime)
{
	UnitSeconds = UnitTime;
	TotalTimeLimit = TotalTime;
	RemainSeconds = TotalTime;
	AccumulatedTime = 0.0f;
	bRunning = true;
	bForever = TotalTime < 0;
}

void ADowncountHelperBase::RestartDowncount()
{
	RemainSeconds = TotalTimeLimit;
	AccumulatedTime = 0.0f;
	bRunning = true;
}

// Called when the game starts
void ADowncountHelperBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void ADowncountHelperBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bRunning)
	{
		AccumulatedTime += DeltaTime;
		if (bForever)
		{
			if (AccumulatedTime > UnitSeconds)
			{
				AccumulatedTime = FMath::Fmod(AccumulatedTime, UnitSeconds);
				this->OnRemainChanged(TotalTimeLimit);
				this->BP_OnRemainChanged(TotalTimeLimit);
			}
		}
		else
		{
			if (RemainSeconds < UnitSeconds)
			{
				bRunning = false;
				this->OnTimeUp();
				this->BP_OnTimeUp();
				return;
			}
			if (AccumulatedTime > UnitSeconds)
			{
				RemainSeconds -= ((int32)(AccumulatedTime / UnitSeconds)) * UnitSeconds;
				AccumulatedTime = FMath::Fmod(AccumulatedTime, UnitSeconds);
				if (RemainSeconds < UnitSeconds)
				{
					bRunning = false;
					this->OnTimeUp();
					this->BP_OnTimeUp();
				}
				else
				{
					this->OnRemainChanged(RemainSeconds);
					this->BP_OnRemainChanged(RemainSeconds);
				}
			}
		}
	}
}
