// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoardIndicator.generated.h"

UCLASS()
class GOBANG_API ABoardIndicator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoardIndicator();

	UPROPERTY(Category = "Properties", VisibleDefaultsOnly, BlueprintReadOnly)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(Category = "Properties", VisibleDefaultsOnly, BlueprintReadOnly)
		class UMaterial* PlayerMaterial;

	UPROPERTY(Category = "Properties", VisibleDefaultsOnly, BlueprintReadOnly)
		class UMaterial* LastMaterial;

	UPROPERTY(Category = "Properties", VisibleDefaultsOnly, BlueprintReadOnly)
		class UMaterial* HintMaterial;

	int CurrentMat;

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void SetIndicatorColor(int Mat);

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void SetIndicatorVisibility(bool Visibility);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
