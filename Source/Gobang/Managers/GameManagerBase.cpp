// This is a private project, cannot be put at commercial use.


#include "GameManagerBase.h"
#include "../Basics/Chess.h"

// Sets default values
AGameManagerBase::AGameManagerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = false;

	Indicator_Player = CreateDefaultSubobject<ABoardIndicator>("Indicator_Player");
	Indicator_Last = CreateDefaultSubobject<ABoardIndicator>("Indicator_Last");
	Indicator_Hint = CreateDefaultSubobject<ABoardIndicator>("Indicator_Hint");

	PlaceChessAudio = CreateDefaultSubobject<USoundWave>("PlaceChessAudio");

}

/*
Prepare the parameters for gameplay.
Return:
	0: AOK, No problem.
	1: The world object is invalid.
	2: The Board Manager is invalid.
	3: The Public Manager is invalid.
*/
int AGameManagerBase::OnGameStart()
{
	UWorld* TheWorld = GetWorld();
	if (TheWorld == nullptr) return 1;
	
	AActor* Temp = UGameplayStatics::GetActorOfClass(TheWorld, ABoardManagerBase::StaticClass());
	if (Temp == nullptr) return 2;
	BoardManager = Cast<ABoardManagerBase>(Temp);
	if (BoardManager == nullptr) return 2;

	Temp = UGameplayStatics::GetActorOfClass(TheWorld, APublicManagerBase::StaticClass());
	if (Temp == nullptr) return 3;
	PublicManager = Cast<APublicManagerBase>(Temp);
	if (PublicManager == nullptr) return 3;

	TArray<AActor*> TempArr;
	UGameplayStatics::GetAllActorsOfClass(TheWorld, AChess::StaticClass(), TempArr);
	for (int i = 0; i < TempArr.Num(); i++) TempArr[i]->Destroy();

	return 0;
}

void AGameManagerBase::SetPlayerIndicator(int32 X, int32 Y, bool IsOk)
{
	if (BoardManager == nullptr) return;
	Indicator_Player->SetActorLocation(BoardManager->GetAbsolutePosition(X, Y, 1.2f));
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("Player indicator set to (%s)"), IsOk ? TEXT("White") : TEXT("Orange")));
	Indicator_Player->SetIndicatorColor(IsOk ? 1 : 2);
	Indicator_Player->SetIndicatorVisibility(true);
}
void AGameManagerBase::HidePlayerIndicator() {
	if (BoardManager == nullptr) return;
	Indicator_Player->SetIndicatorVisibility(false);
}
void AGameManagerBase::SetHintIndicator(int32 X, int32 Y)
{
	if (BoardManager == nullptr) return;
	Indicator_Hint->SetActorLocation(BoardManager->GetAbsolutePosition(X, Y, 1.1f));
	Indicator_Hint->SetIndicatorVisibility(true);
}
void AGameManagerBase::HideHintIndicator() {
	if (BoardManager == nullptr) return;
	Indicator_Hint->SetIndicatorVisibility(false);
}
void AGameManagerBase::SetLastIndicator(int32 X, int32 Y)
{
	if (BoardManager == nullptr) return;
	Indicator_Last->SetActorLocation(BoardManager->GetAbsolutePosition(X, Y, 1.0f));
	Indicator_Last->SetIndicatorVisibility(true);
}
void AGameManagerBase::HideLastIndicator() {
	if (BoardManager == nullptr) return;
	Indicator_Last->SetIndicatorVisibility(false);
}

// Called when the game starts or when spawned
void AGameManagerBase::BeginPlay()
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
void AGameManagerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

