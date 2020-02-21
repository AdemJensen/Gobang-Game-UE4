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
		virtual void OnRoundStart() { }

	UFUNCTION(BlueprintCallable, Category = "Game Actions")
		void OnRoundOver();

	UFUNCTION(BlueprintCallable, Category = "Game Actions")
		void OnSurrounder();

	UFUNCTION(BlueprintCallable, Category = "Game Actions")
		void OnUnexpectedAction(EUnexpectedGameActionType Type);

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
