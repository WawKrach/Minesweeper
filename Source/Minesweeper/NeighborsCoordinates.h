// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Coordinates.h"
#include "NeighborsCoordinates.generated.h"

/**
 *
 */
UCLASS()
class MINESWEEPER_API UNeighborsCoordinates : public UObject
{
	GENERATED_BODY()

public:

	FCoordinates NeighbourA
	{
		NeighbourA.X = -1,
		NeighbourA.Y = 1
	};
	FCoordinates NeighbourB
	{
		NeighbourB.X = 0,
		NeighbourB.Y = 1
	};
	FCoordinates NeighbourC
	{
		NeighbourC.X = 1,
		NeighbourC.Y = 1
	};
	FCoordinates NeighbourD
	{
		NeighbourD.X = -1,
		NeighbourD.Y = 0
	};
	FCoordinates NeighbourE
	{
		NeighbourE.X = 1,
		NeighbourE.Y = 0
	};
	FCoordinates NeighbourF
	{
		NeighbourF.X = -1,
		NeighbourF.Y = -1
	};
	FCoordinates NeighbourG
	{
		NeighbourG.X = 0,
		NeighbourG.Y = -1
	};
	FCoordinates NeighbourH
	{
		NeighbourH.X = 1,
		NeighbourH.Y = -1
	};

	FCoordinates Neighbours[8]{ NeighbourA,NeighbourB,NeighbourC,NeighbourD,NeighbourE,NeighbourF,NeighbourG,NeighbourH };


};
