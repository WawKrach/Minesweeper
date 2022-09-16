// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Coordinates.h"
#include "FieldStaticMeshComponent.generated.h"



UCLASS(Blueprintable, ClassGroup = (Rendering, Common), hidecategories = (Object, Activation, "Components|Activation"), ShowCategories = (Mobility), editinlinenew, meta = (BlueprintSpawnableComponent))
class MINESWEEPER_API UFieldStaticMeshComponent : public UStaticMeshComponent
{
	GENERATED_UCLASS_BODY()
public:
		UFieldStaticMeshComponent();

	UPROPERTY(EditAnywhere);
		UStaticMesh* DefaultMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Minesweeper);
		UMaterialInterface* DefaultMaterial;

	UPROPERTY(BlueprintReadWrite, Category = Minesweeper);
		UMaterialInstanceDynamic* FieldMaterial;

	void CreateFieldMaterialInstance();

	void SetCoordinates(FCoordinates NewCoordinates);
	
	FCoordinates Coordinates;	

	void SetWorldLocationByCoordinates();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MinesWeeper")
		float FieldSpace = 200;

	bool bRevealed;

	UFUNCTION(BlueprintNativeEvent)
		void Reveal();

	UPROPERTY(BlueprintReadOnly, Category = "MinesWeeper")
		bool bFlaged;

	UPROPERTY(BlueprintReadOnly, Category = "MinesWeeper")
		bool bMined;

	UPROPERTY(BlueprintReadOnly, Category = "MinesWeeper")
		int32 NeightbourMinesCounter;

	void AddNeighbourMine();

	UFUNCTION(BlueprintNativeEvent)
		void ShowFlag();

	UFUNCTION(BlueprintNativeEvent)
		void TakeFlag();

	UFUNCTION(BlueprintNativeEvent)
		void ResetField();

};
