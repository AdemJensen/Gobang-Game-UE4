// This is a private project, cannot be put at commercial use.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DowncountHelperBase.h"
#include "BlinkHelper.generated.h"


DECLARE_MULTICAST_DELEGATE(FActionDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActionDelegateBP);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GOBANG_API ABlinkHelper : public ADowncountHelperBase
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ABlinkHelper();

	/*
	In one gap:
		- Do ShowSign();
		- Sustain;
		- Do HideSign();
		- Wait for the rest of gap time.
	Do [RepeteTimes] gaps.
	*/
	virtual void StartBlink(float GapTime, float SustainTime, int32 RepeteTimes, bool bDoLastHidden = true);

	FActionDelegate ShowSignDelegate;
	FActionDelegate HideSignDelegate;

	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "Info")
		FActionDelegateBP BP_ShowSignDelegate;
	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "Info")
		FActionDelegateBP BP_HideSignDelegate;

protected:

	UPROPERTY(VisibleAnywhere, Category = "Info")
		float Gap = 1.0f;
	UPROPERTY(VisibleAnywhere, Category = "Info")
		float Sustain = 0.5f;
	UPROPERTY(VisibleAnywhere, Category = "Info")
		int32 RepeteRemain = 3;
	UPROPERTY(VisibleAnywhere, Category = "Info")
		bool bDoLast = true;

	UPROPERTY(VisibleAnywhere, Category = "Info")
		bool bInSustain = false;

	virtual void OnTimeUp();
		
};
