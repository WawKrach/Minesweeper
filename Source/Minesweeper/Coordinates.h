// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Misc/Crc.h"
#include "Coordinates.generated.h"


USTRUCT(BlueprintType)
struct FCoordinates
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
		int32 X;
	UPROPERTY(BlueprintReadWrite)
		int32 Y;

    //Overload operator to compare structs
	bool operator==(const FCoordinates& Other) const
	{
		return Equals(Other);
	}

	bool Equals(const FCoordinates& Other) const
	{
		bool IsEqual = false;
		return (X == Other.X) && (Y == Other.Y) ? true : false;
	}

};

////Necessary to use this struct as a TMap Key
#if UE_BUILD_DEBUG
uint32 GetTypeHash(const FCoordinates& Thing);
#else // optimize by inlining in shipping and development builds
FORCEINLINE uint32 GetTypeHash(const FCoordinates& Thing)
{
	uint32 Hash = FCrc::MemCrc32(&Thing, sizeof(FCoordinates));
	return Hash;
}
#endif