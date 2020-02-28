// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Math/Vector2D.h"
#include "../Managers/BoardManagerBase.h"
#include "BoardTrigger.generated.h"

UCLASS()
class GOBANG_API ABoardTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoardTrigger();

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void InitUtility(FVector2D size, int32 Coord_X, int32 Coord_Y, ABoardManagerBase* WorldOwnerBoard);

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void SetTaken(bool Taken) { IsTaken = Taken; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* Existance;

	UFUNCTION()
		void TriggerClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked);
	UFUNCTION()
		void OnFingerPressedTrigger(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
		void HandleClick();

	UFUNCTION()
		void TimerAction();

	int32 Coordinate_X;
	int32 Coordinate_Y;
	bool IsTaken = false;

	ABoardManagerBase* OwnerBoard;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
