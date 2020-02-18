// This is a private project, cannot be put at commercial use.


#include "PublicManagerBase.h"

// Sets default values
APublicManagerBase::APublicManagerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
}

// Called when the game starts or when spawned
void APublicManagerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APublicManagerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

