// Copyright Epic Games, Inc. All Rights Reserved

#include "MinesweeperGameModeBase.h"
#include "Misc/DateTime.h"
#include "Json.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"


void AMinesweeperGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	PlayerID = CreatePlayerID();

	TArray<AActor*> Grids;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGrid::StaticClass(), Grids);
	Grid = (AGrid*)Grids[0];
	   	 
	if (!FModuleManager::Get().IsModuleLoaded("WebSockets"))
	{
		FModuleManager::Get().LoadModule("WebSockets");
	}

	WebSocket = FWebSocketsModule::Get().CreateWebSocket("ws://localhost:8080");

	if (!WebSocket)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Create WebSocket Failed");
	}

	WebSocket->OnConnected().AddLambda([]()
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Successfully connected");
	});

	WebSocket->OnConnectionError().AddLambda([](const FString& Error)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, Error);
	});

	WebSocket->OnClosed().AddLambda([](int32 StatusCode, const FString& Reason, bool bWasClean)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, bWasClean ? FColor::Green : FColor::Red, "Connection closed " + Reason);
	});

	WebSocket->OnMessageSent().AddLambda([](const FString& MessageString)
	{
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "Sent message: " + MessageString);
	});


	WebSocket->OnMessage().AddUObject(this, &AMinesweeperGameModeBase::ReceiveMessage);
	WebSocket->OnMessage().AddUObject(this, &AMinesweeperGameModeBase::ReciveAction);


	WebSocket->Connect();



}



FString AMinesweeperGameModeBase::CreatePlayerID()
{
	return FDateTime::Now().ToString();
}



void AMinesweeperGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (WebSocket->IsConnected())
	{
		WebSocket->Close();
	}
	Super::EndPlay(EndPlayReason);
}



void AMinesweeperGameModeBase::ClickedLeft(FCoordinates ClickCoords)
{
	switch (CurrentGameStatus)
	{
		case EGameStatus::GameReadyToStart:
			StartGame(ClickCoords);
			break;
		case EGameStatus::GameInProgress:
			//Generate PlayStructure
			break;
		default: break;
	}

}

void AMinesweeperGameModeBase::ClickedRight(FCoordinates ClickCoords)
{
	switch (CurrentGameStatus)
	{
	case EGameStatus::GameInProgress:
		PutFlag(ClickCoords);
		break;
	}
}


void AMinesweeperGameModeBase::StartGame(FCoordinates FirstClickCoords)
{
	if (Grid)
	{
		SendAction(Grid->GenerateCoordianateOfAllMines(FirstClickCoords));
	}
}

void AMinesweeperGameModeBase::RevealField(FCoordinates ClickCoords)
{
	if (Grid)
	{
//		SendAction(Grid->CheckField(ClickCoords));
	}
}

void AMinesweeperGameModeBase::PutFlag(FCoordinates ClickCoords)
{
	if (Grid)
	{
//		SendAction(Grid->CheckFlag(ClickCoords));
	}
}

void AMinesweeperGameModeBase::EndGame()
{

}


//Tu dostajemy kordynaty, które musimy spakować wraz z ID i Krokiem w rozgrywce
void AMinesweeperGameModeBase::SendAction(TArray<FCoordinates> Coords)
{

	TArray<FJsonDataContainer> DataContainers; //-> Fullfill with coords
	FJsonData NewJsonData = FJsonData(DataContainers);
}


///Muszę zrobić coś takiego:
/*
Recive action wychwytuje za pomocą switcha akcję, ta odpala odpowiednią funkcję na gridzie lokalnie (która jest tamtejszą rozciętą funkcją).
ale wcześniej musi ją odpowiednio zdekodować na strukturę z powrotem


*/



void AMinesweeperGameModeBase::ReciveAction(const FString& MessageString)
{

}


void AMinesweeperGameModeBase::ReciveStartGame(FCoordinates FirstClickCoords)
{

}
void AMinesweeperGameModeBase::ReciveRevealField(FCoordinates ClickCoords)
{

}
void AMinesweeperGameModeBase::RecivePutFlag(FCoordinates ClickCoords)
{

}
void AMinesweeperGameModeBase::ReciveEndGame()
{

}






void AMinesweeperGameModeBase::SendMessage()
{
	TArray<FJsonDataContainer> DataContainers;

	FCoordinates NewCoordinates;
	NewCoordinates.X = 11;
	NewCoordinates.Y = 22;

	FJsonDataContainer DataContainer("1671717", NewCoordinates, CurrentGameStatus); 
	DataContainers.Add(DataContainer);

	DataContainer.userId = "1831";
	NewCoordinates.X = 33;
	NewCoordinates.Y = 44;
	DataContainer.Coords = NewCoordinates;
	DataContainers.Add(DataContainer);

	FJsonData NewJsonData = FJsonData(DataContainers);
	FString JSONPayload;
	FJsonObjectConverter::UStructToJsonObjectString(NewJsonData, JSONPayload, 0, 0);

	WebSocket->Send(JSONPayload);
}


void AMinesweeperGameModeBase::ReceiveMessage(const FString& MessageString)
{
	FJsonData JSonData = FJsonData(MessageString);
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Variable : x: %i"), JSonData.Employees[0].Coords.X));
	//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Coords: Actor: %s"), *J));

}




/*
	Co chcę zakodować?

	Chcę stworzyć strukturę, która będzie interpretowana zależnie od ustawionego GameState'u.

	Na wysyłce:
		-> jeśli wiadomość ma GameReadyToStart, to przekażę tablicę losowych pól
		-> jeśli wiadomość ma GameInProgress, to przekazę pojedynczą koordynatę
	Na odbiorze:
		-> jeśli wiadomość ma GameReadyToStart, a lokalnie stan jest różny od GameReadyToStart, to najpierw resetuje rozgrywkę
		-> jeśli wiadomość ma GameReadyToStart, to zmieni się na GameInProgress i rozłoży miny zgodnie z przesłanym parameterem
		-> jeśli wiadomość ma GameInProgress, to odsłonięta zostanie przesłana koordynata, czyli wzięta pierwsza z brzegu

		-> Jeśli wybuchnie mina, to obie gry po delayu się resetują (nie trzeba tego przesyłać, bo wszystko jest rozsyłane)

	Przy nadaniu zawsze sprawdzam, czy ID nadawcy akcji różni się od ostatnio przyjętego ID akcji.
*/


//Jedno to jest upakowanie do structa, załatwię to tutaj, ok. Ale tak naprawdę nie odpowiedziałem na centralne pytanie,
//jak karmić tą funkcję...
//Potrzebuję funkcji..




