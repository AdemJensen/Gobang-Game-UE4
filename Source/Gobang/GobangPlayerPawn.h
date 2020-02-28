// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Engine.h"
#include "GobangPlayerPawn.generated.h"

UCLASS()
class GOBANG_API AGobangPlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGobangPlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void TraceForBlock(const FVector& Start, const FVector& End, bool bDrawDebugHelpers);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
