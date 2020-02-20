// This is a private project, cannot be put at commercial use.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "DowncountHelperBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GOBANG_API ADowncountHelperBase : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ADowncountHelperBase();

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void StartDowncount(float TotalTime, float UnitTime = 1.0f);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Actions")
		void OnRemainChanged(float TimeRemain);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Actions")
		void OnTimeUp();

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void Interrupt() { bRunning = false; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	float UnitSeconds = 1.0f;
	float RemainSeconds = 0.0f;
	float AccumulatedTime = 0.0f;

	bool bRunning = false;

	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

		
};
