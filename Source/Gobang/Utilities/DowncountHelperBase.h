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

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void ContinueDowncount() { bRunning = true; }

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void RestartDowncount();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Actions")
		void BP_OnRemainChanged(float TimeRemain);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Actions")
		void BP_OnTimeUp();
	UFUNCTION(BlueprintCallable, Category = "Actions")
		virtual void OnRemainChanged(float TimeRemain) { }
	UFUNCTION(BlueprintCallable, Category = "Actions")
		virtual void OnTimeUp() { }

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void Interrupt() { bRunning = false; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Info")
		float UnitSeconds = 1.0f;
	UPROPERTY(VisibleAnywhere, Category = "Info")
		float RemainSeconds = 0.0f;
	UPROPERTY(VisibleAnywhere, Category = "Info")
		float TotalTimeLimit = 0.0f;
	UPROPERTY(VisibleAnywhere, Category = "Info")
		float AccumulatedTime = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = "Info")
		bool bRunning = false;
	UPROPERTY(VisibleAnywhere, Category = "Info")
		bool bForever = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

		
};
