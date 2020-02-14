// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "GobangChess.generated.h"

UCLASS()
class GOBANG_API AGobangChess : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGobangChess();

	UPROPERTY(Category = "Properties", VisibleDefaultsOnly, BlueprintReadOnly)
		class UStaticMeshComponent* Mesh;

	class UStaticMesh* Mesh_Black;
	class UStaticMesh* Mesh_White;

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void SetChessVisibility(bool Visibility);

	bool IsColoredNow;

	void SetChessColor(bool IsBlack);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
