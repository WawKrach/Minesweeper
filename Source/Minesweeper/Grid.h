// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FieldStaticMeshComponent.h"
#include "Coordinates.h"
#include "NeighborsCoordinates.h"
#include "Grid.generated.h"

UCLASS()
class MINESWEEPER_API AGrid : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	void BindMouse();

	//Component class expanded as blueprint
	UPROPERTY(BlueprintReadWrite)
		TSubclassOf<class UFieldStaticMeshComponent> FieldToCreate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MinesWeeper")
		int32 GridSize = 16;
	   
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MinesWeeper")
		int32 MinesNumber = 10;

	//Number off pulled fields;
	const int32 MaxFieldNumber = 2048;//It looks like that this is the size limit of TMap 

	int32 MaxGridSize;

	//Total pool of Components (with no static mesh by defualt)
	UPROPERTY(EditAnywhere);
		TArray<UFieldStaticMeshComponent*> FieldComponents;

	//Map of active components in game
	UPROPERTY(BlueprintReadOnly, Category = "MinesWeeper")
		TMap<FCoordinates, UFieldStaticMeshComponent* > Fields;

	//Object that contain array of FCoordinates around clicked point, that means +/- 1 in x/y around field. 
	UPROPERTY()
	UNeighborsCoordinates* NeighborCoordinatesTemplate;

	//Manage the game
	UFUNCTION(BlueprintCallable, Category = "MinesWeeper")
		void StarGame();

	UFUNCTION(BlueprintCallable, Category = "MinesWeeper")
		void SetGridSize(int NewGridSize);

	UFUNCTION(BlueprintCallable, Category = "MinesWeeper")
		void ResetGrid();

	UFUNCTION(BlueprintCallable, Category = "MinesWeeper")
		void SetMinesNumber(int NewMinesNumber);

	//Depend from GridSize 
	void SetFieldsCoordinates();
	void SetFieldsOnGrid();
	
	//Depend from first click and number of mines
	TArray<FCoordinates> GenerateCoordianateOfAllMines(FCoordinates FirstClickCoordinates);
	void PlaceMines(TArray<FCoordinates> MinedFieldCoords);
	void MarkMinesNeighbours(UFieldStaticMeshComponent* MinedField);

	//Interaction with fields
	void CheckField(UFieldStaticMeshComponent* FieldToCheck);
	void CheckFieldNeighbors(UFieldStaticMeshComponent* FieldToCheck);
	void CheckFlag(UFieldStaticMeshComponent* FieldToCheck);
	void MarkFieldWithFlag(UFieldStaticMeshComponent* FieldToMark);

	UFUNCTION(BlueprintCallable, Category = "MinesWeeper")
	void CheckFieldByCoordinates(FCoordinates Coordinates);
};

