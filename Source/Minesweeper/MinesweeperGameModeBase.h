// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WebSocketsModule.h" // Module definition
#include "IWebSocket.h"       // Socket definition
#include "Coordinates.h"
#include "Grid.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"//It must be in the structure
#include "MinesweeperGameModeBase.generated.h"


UENUM()
enum EGameStatus
{
	GameReadyToStart     UMETA(DisplayName = "GameReadyToStart"),
	GameInProgress      UMETA(DisplayName = "GameInProgress"),
};

USTRUCT(BlueprintType)
struct FJsonDataContainer
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EGameStatus> CurrentGameStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString userId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCoordinates Coords;
	
	FJsonDataContainer() {};

	FJsonDataContainer(FString _userId, FCoordinates _Coords, EGameStatus _CurrentGameStatus)
	{
		CurrentGameStatus = _CurrentGameStatus;
		userId = _userId;
		Coords = _Coords;
	}
};

USTRUCT(BlueprintType)
struct FJsonData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FJsonDataContainer> Employees;

	FJsonData() {};

	//Podajê parametr, ktory jest odczytywany przez deserializator
	FJsonData(TArray<FJsonDataContainer> _Employees) //To jest serializator
	{
		Employees = _Employees;
	}

	//jeœli ten struct dostaje stringa jako parametr przy konstrukcji (zwróc uwage jak latwo tworzyc obiekty ustawianie na starcie w ten sposob.) 
	//..to wywoluje funkcje, ktora przeksztalca parametr wejsciowy na czytelna struktre. Zajebiste.
	FJsonData(FString _json_) 
	{
		FJsonData _tmpJsonData;

		FJsonObjectConverter::JsonObjectStringToUStruct<FJsonData>(_json_, &_tmpJsonData, 0, 0);
		Employees = _tmpJsonData.Employees;
	}
};





UCLASS()
class MINESWEEPER_API AMinesweeperGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	

	UPROPERTY(BlueprintReadWrite)
		AGrid* Grid;

	void ClickedRight(FCoordinates ClickCoords);
	void ClickedLeft(FCoordinates ClickCoords);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<EGameStatus> CurrentGameStatus;

	FString PlayerID;
	FString CreatePlayerID();

	void StartGame(FCoordinates FirstClickCoords);
	void RevealField(FCoordinates ClickCoords);
	void PutFlag(FCoordinates ClickCoords);
	void EndGame();

	void SendAction(TArray<FCoordinates> Coord);

	void ReciveAction(const FString& MessageString);

	void ReciveStartGame(FCoordinates FirstClickCoords);
	void ReciveRevealField(FCoordinates ClickCoords);
	void RecivePutFlag(FCoordinates ClickCoords);
	void ReciveEndGame();



	TSharedPtr<IWebSocket> WebSocket;
	void SendMessage();
	void ReceiveMessage(const FString& MessageString);

};


