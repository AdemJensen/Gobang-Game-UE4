// This is a private project, cannot be put at commercial use.


#include "IndicationManager.h"
#include "BoardManagerBase.h"

// Sets default values
AIndicationManager::AIndicationManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Indicator_Player = CreateDefaultSubobject<ABoardIndicator>("Indicator_Player");
	Indicator_Last = CreateDefaultSubobject<ABoardIndicator>("Indicator_Last");
	Indicator_Hint = CreateDefaultSubobject<ABoardIndicator>("Indicator_Hint");

	PlaceChessSuccessAudio = CreateDefaultSubobject<USoundWave>("PlaceChessSuccessAudio");
	PlaceChessFailAudio = CreateDefaultSubobject<USoundWave>("PlaceChessFailAudio");
}

void AIndicationManager::SetPlayerIndicator(int32 X, int32 Y, bool IsOk)
{
	Indicator_Player->SetActorLocation(GetAbsolutePosition(X, Y, 1.2f));
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("Player indicator set to (%s)"), IsOk ? TEXT("White") : TEXT("Orange")));
	Indicator_Player->SetIndicatorColor(IsOk ? 1 : 2);
	Indicator_Player->SetIndicatorVisibility(true);
}
void AIndicationManager::HidePlayerIndicator() {
	Indicator_Player->SetIndicatorVisibility(false);
}
void AIndicationManager::SetHintIndicator(int32 X, int32 Y)
{
	Indicator_Hint->SetActorLocation(GetAbsolutePosition(X, Y, 1.1f));
	Indicator_Hint->SetIndicatorVisibility(true);
}
void AIndicationManager::HideHintIndicator() {
	Indicator_Hint->SetIndicatorVisibility(false);
}
void AIndicationManager::SetLastIndicator(int32 X, int32 Y)
{
	Indicator_Last->SetActorLocation(GetAbsolutePosition(X, Y, 1.0f));
	Indicator_Last->SetIndicatorVisibility(true);
}
void AIndicationManager::HideLastIndicator() {
	Indicator_Last->SetIndicatorVisibility(false);
}

FVector AIndicationManager::GetAbsolutePosition(int32 X, int32 Y, float Z_Offset)
{
	float boardX = Locator_RD->Location->GetComponentLocation().X - Locator_LU->Location->GetComponentLocation().X;
	float boardY = Locator_RD->Location->GetComponentLocation().Y - Locator_LU->Location->GetComponentLocation().Y;
	float unitX = boardX / (ABoardManagerBase::BOARD_SIZE - 1);
	float unitY = boardY / (ABoardManagerBase::BOARD_SIZE - 1);
	return FVector(
		Locator_LU->Location->GetComponentLocation().X + unitX * X,
		Locator_LU->Location->GetComponentLocation().Y + unitY * Y,
		Locator_LU->Location->GetComponentLocation().Z + Z_Offset
	);
}

void AIndicationManager::PlayChessSound(bool bSuccess, FVector Location)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), bSuccess ? PlaceChessSuccessAudio : PlaceChessFailAudio, Location);
}

// Called when the game starts or when spawned
void AIndicationManager::BeginPlay()
{
	Super::BeginPlay();
	
	UWorld* TheWorld = GetWorld();

	Indicator_Player = TheWorld->SpawnActor<ABoardIndicator>(FVector(0, 0, 0), FRotator(0, 0, 0));
	Indicator_Last = TheWorld->SpawnActor<ABoardIndicator>(FVector(0, 0, 0), FRotator(0, 0, 0));
	Indicator_Hint = TheWorld->SpawnActor<ABoardIndicator>(FVector(0, 0, 0), FRotator(0, 0, 0));
	Indicator_Player->SetIndicatorColor(1);
	Indicator_Last->SetIndicatorColor(2);
	Indicator_Hint->SetIndicatorColor(3);
	Indicator_Player->SetIndicatorVisibility(false);
	Indicator_Last->SetIndicatorVisibility(false);
	Indicator_Hint->SetIndicatorVisibility(false);
}

// Called every frame
void AIndicationManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

