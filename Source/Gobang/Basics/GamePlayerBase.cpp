// This is a private project, cannot be put at commercial use.


#include "GamePlayerBase.h"

// Sets default values
AGamePlayerBase::AGamePlayerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = false;
}

// Called when the game starts or when spawned
void AGamePlayerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGamePlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

