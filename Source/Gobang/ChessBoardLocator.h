// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "ChessBoardLocator.generated.h"

UCLASS()
class GOBANG_API AChessBoardLocator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChessBoardLocator();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Location;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
