// This is a private project, cannot be put at commercial use.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Basics/GameStage.h"
#include "../Basics/ChessType.h"
#include "PublicManagerBase.generated.h"

UCLASS()
class GOBANG_API APublicManagerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APublicManagerBase();

	UFUNCTION(BlueprintCallable, Category = "Game Status")
		void SetGameStage(EGameStage GameStage) { Stage = GameStage; }
	UFUNCTION(BlueprintCallable, Category = "Game Status")
		EGameStage GetGameStage() { return Stage; }

	UFUNCTION(BlueprintCallable, Category = "Game Info")
		void SetSelfChess(EChessType ChessType) { SelfChess = ChessType; }
	UFUNCTION(BlueprintCallable, Category = "Game Info")
		EChessType GetSelfChess() { return SelfChess; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Game Status")
		EGameStage Stage = EGameStage::UNKNOWN;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Info")
		EChessType SelfChess = EChessType::UNKNOWN;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
