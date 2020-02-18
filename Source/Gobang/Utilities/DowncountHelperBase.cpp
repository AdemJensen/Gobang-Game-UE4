// This is a private project, cannot be put at commercial use.


#include "DowncountHelperBase.h"

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
	RemainSeconds = TotalTime;
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
		if (AccumulatedTime > UnitSeconds)
		{
			RemainSeconds -= ((int32) (AccumulatedTime / UnitSeconds)) * UnitSeconds;
			AccumulatedTime = FMath::Fmod(AccumulatedTime, UnitSeconds);
			if (RemainSeconds < UnitSeconds)
			{
				bRunning = false;
				OnTimeUp();
			}
			else
			{
				OnRemainChanged(RemainSeconds);
			}
		}
	}
}
