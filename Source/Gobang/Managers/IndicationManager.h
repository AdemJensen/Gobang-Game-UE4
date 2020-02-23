// This is a private project, cannot be put at commercial use.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Basics/BoardIndicator.h"
#include "../Basics/BoardLocator.h"
#include "IndicationManager.generated.h"

UCLASS()
class GOBANG_API AIndicationManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIndicationManager();

	// Indicators
	UFUNCTION(BlueprintCallable, Category = "Presentations")
		void SetPlayerIndicator(int32 X, int32 Y, bool IsOk);
	UFUNCTION(BlueprintCallable, Category = "Presentations")
		void SetHintIndicator(int32 X, int32 Y);
	UFUNCTION(BlueprintCallable, Category = "Presentations")
		void SetLastIndicator(int32 X, int32 Y);
	UFUNCTION(BlueprintCallable, Category = "Presentations")
		void HideHintIndicator();
	UFUNCTION(BlueprintCallable, Category = "Presentations")
		void HidePlayerIndicator();
	UFUNCTION(BlueprintCallable, Category = "Presentations")
		void HideLastIndicator();

	UFUNCTION(BlueprintCallable, Category = "Game Info")
		FVector GetAbsolutePosition(int32 X, int32 Y, float Z_Offset = 0);

	UFUNCTION(BlueprintCallable, Category = "Presentations")
		void PlayChessSound(bool bSuccess, FVector Location);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Presentations")
		ABoardLocator* Locator_LU;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Presentations")
		ABoardLocator* Locator_RD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Presentations")
		ABoardIndicator* Indicator_Player;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Presentations")
		ABoardIndicator* Indicator_Last;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Presentations")
		ABoardIndicator* Indicator_Hint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Presentations")
		USoundWave* PlaceChessSuccessAudio;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Presentations")
		USoundWave* PlaceChessFailAudio;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
