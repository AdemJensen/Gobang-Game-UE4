// This is a private project, cannot be put at commercial use.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Basics/UnexpectedGameActionType.h"
#include "GamePlayerType.h"
#include "../Basics/ChessType.h"
#include "GamePlayerBase.generated.h"

UCLASS()
class GOBANG_API AGamePlayerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGamePlayerBase();

	UFUNCTION(BlueprintCallable, Category = "Game Actions")
		virtual void OnGameStart() { }	// Called outside, needs to implement inside.

	UFUNCTION(BlueprintCallable, Category = "Game Actions")
		virtual void OnRoundStart() { }	// Called outside, needs to implement inside.

	UFUNCTION(BlueprintCallable, Category = "Game Actions")
		virtual void OnInterrupt() { }	// Called outside, needs to implement inside.

	/**
	This function is called when either of the player called "DoRetract."
	*/
	UFUNCTION(BlueprintCallable, Category = "Game Actions")
		virtual void OnRetract(FIntPoint RetractPosition) { }	// Called outside, needs to implement inside.

	UFUNCTION(BlueprintCallable, Category = "Game Info")
		void SetChessType(EChessType Type) { ChessType = Type; }
	UFUNCTION(BlueprintCallable, Category = "Game Info")
		EChessType GetChessType() { return ChessType; }

	UFUNCTION(BlueprintCallable, Category = "Game Info")
		virtual EGamePlayerType GetPlayerType() { return EGamePlayerType::UNKNOWN; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		AGameModeBase* GetGameMode();

	UPROPERTY(EditAnywhere, Category = "Game Info")
		EChessType ChessType = EChessType::UNKNOWN;

	UFUNCTION(BlueprintCallable, Category = "Game Actions")
		void DoRoundOver(FIntPoint ActionLocation);

	UFUNCTION(BlueprintCallable, Category = "Game Actions")
		void DoSurrounder();

	UFUNCTION(BlueprintCallable, Category = "Game Actions")
		void DoUnexpectedAction(EUnexpectedGameActionType Type);

	UFUNCTION(BlueprintCallable, Category = "Game Actions")
		void DoRetract();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
