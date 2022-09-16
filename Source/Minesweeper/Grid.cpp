// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "MinesweeperPlayerController.h"
#include "MinesweeperGameModeBase.h"
#include "Kismet/BlueprintFunctionLibrary.h" 



DECLARE_STATS_GROUP(TEXT("LODZERO_Game"), STATGROUP_LODZERO, STATCAT_Advanced);
// DisplayName, GroupName (ends up as: "LODZERO"), Third param is always Advanced.D

DECLARE_CYCLE_STAT(TEXT("GetModuleByClass (Single)"), STAT_GetSingleModuleByClass, STATGROUP_LODZERO);

//It's good for the performance to have this actor on the map already cause it's pooling components.
AGrid::AGrid()
{
	PrimaryActorTick.bCanEverTick = false;

	//Find blueprint of FieldStaticMeshVomponent, to extend visual functionalities there
	static ConstructorHelpers::FClassFinder<UFieldStaticMeshComponent> ItemBlueprint(TEXT("/Game/Blueprints/FieldStaticMeshComponentBP"));
	if (ItemBlueprint.Class != NULL)
	{
		FieldToCreate = ItemBlueprint.Class;
	}

	//Create pool of components so it would be ready to set static meshes and locations latter 
	if (IsValid(FieldToCreate))
	{
		for (int32 i = 0; i < MaxFieldNumber; i++)
		{
			UFieldStaticMeshComponent* NewField = NewObject<UFieldStaticMeshComponent>(this, FieldToCreate, *FString("Mesh" + FString::FromInt(i)));
			if (NewField)
			{
				FieldComponents.Add(NewField);
			}
		}
	}

	MaxGridSize = FMath::Sqrt(MaxFieldNumber);
	FMath::Floor(MaxGridSize);

	//NeighborCoordinatesTemplate = NewObject<UNeighborsCoordinates>(this, UNeighborsCoordinates::StaticClass(), "NeighborsCoord");
	NeighborCoordinatesTemplate = CreateDefaultSubobject<UNeighborsCoordinates>(TEXT("NeighborsCoord"));
}


void AGrid::BeginPlay()
{
	Super::BeginPlay();
}


void AGrid::StarGame()
{
	SetFieldsCoordinates();
	SetFieldsOnGrid();
	SetActorLocation(FVector::ZeroVector);

	AMinesweeperGameModeBase * GameMode = (AMinesweeperGameModeBase *)GetWorld()->GetAuthGameMode();

	if (GameMode)
	{
		GameMode->SendMessage();

	}
}


void AGrid::SetFieldsCoordinates()
{
	FCoordinates NewCoordinates;
	int32 FieldCounter = 0;

	for (int32 i = 0; i < GridSize; i++)
	{
		NewCoordinates.X = i;//Find next coordinate in column
		for (int32 ii = 0; ii < GridSize; ii++)
		{
			if (FieldComponents[FieldCounter])
			{
				NewCoordinates.Y = ii;//Find next coordinate in row
				FieldComponents[FieldCounter]->SetCoordinates(NewCoordinates);//Set coordinates inside the component
				Fields.Add(NewCoordinates, FieldComponents[FieldCounter]);//Fill the map with coordinates 
				FieldCounter++;
			}
		}
	}
}


void AGrid::SetFieldsOnGrid()
{
	UFieldStaticMeshComponent* CurrentField;			//SCOPE_CYCLE_COUNTER(STAT_GetSingleModuleByClass);//Macro to measure performance by commandline 
	TArray<FCoordinates> FieldCoordinates;
	Fields.GetKeys(FieldCoordinates);
	for (FCoordinates Coord : FieldCoordinates)
	{
		CurrentField = Cast<UFieldStaticMeshComponent>(*Fields.Find(Coord));

		CurrentField->SetStaticMesh(CurrentField->DefaultMesh);
		CurrentField->CreateFieldMaterialInstance();
		CurrentField->SetWorldLocationByCoordinates();
	}
}

TArray<FCoordinates> AGrid::GenerateCoordianateOfAllMines(FCoordinates FirstClickCoordinates)
{
	TArray<FCoordinates> MinedFieldCoords;
	FCoordinates MinesCoordinates;
	int32 SettedMines = 0;

	//Rand mined field coordinates
	for (int32 i = 0; i < MinesNumber; i = SettedMines)
	{
		MinesCoordinates.X = FMath::RandRange(0, GridSize - 1);
		MinesCoordinates.Y = FMath::RandRange(0, GridSize - 1);

		//Check if there is a mine already there...
		if (!MinedFieldCoords.Contains(MinesCoordinates) && !MinesCoordinates.Equals(FirstClickCoordinates))
		{
			MinedFieldCoords.Add(MinesCoordinates);
			SettedMines++;
		}
	}

	return MinedFieldCoords;
	
	//PlaceMines(MinedFieldCoords); //-->To Recive
}

void AGrid::PlaceMines(TArray<FCoordinates> MinedFieldCoords)// ----> To muszê wyrzuciæ do funkcji na zewn¹trz
{

	UFieldStaticMeshComponent* FieldWithMine;
	for (FCoordinates Coord : MinedFieldCoords) //// Do multi muszê to wypchn¹æ do funkcji. z parametrem, który bêdê móg³ przekazaæ w serializowanej formie. 
	{
		if (Fields.Find(Coord))
		{
			FieldWithMine = Cast<UFieldStaticMeshComponent>(*Fields.Find(Coord));
			FieldWithMine->bMined = true;
			MarkMinesNeighbours(FieldWithMine);
		}
	}
}



void AGrid::MarkMinesNeighbours(UFieldStaticMeshComponent* MinedField)
{
	if (!MinedField->IsValidLowLevel())
	{
		return;
	}

	FCoordinates NeighborCoord;
	UFieldStaticMeshComponent* NeighborField = nullptr;
	for (FCoordinates CoordinateAroundMine : NeighborCoordinatesTemplate->Neighbours)
	{
		NeighborCoord.X = MinedField->Coordinates.X + CoordinateAroundMine.X;
		NeighborCoord.Y = MinedField->Coordinates.Y + CoordinateAroundMine.Y;

		if (Fields.Find(NeighborCoord))
		{
			Cast<UFieldStaticMeshComponent>(*Fields.Find(NeighborCoord))->AddNeighbourMine();
		}
	}
}


void AGrid::CheckFieldNeighbors(UFieldStaticMeshComponent* FieldToCheck)
{
	FCoordinates NeighborCoord;

	//Check fields around clicked field
	for (FCoordinates CoordinateAroundMine : NeighborCoordinatesTemplate->Neighbours)
	{
		NeighborCoord.X = FieldToCheck->Coordinates.X + CoordinateAroundMine.X;
		NeighborCoord.Y = FieldToCheck->Coordinates.Y + CoordinateAroundMine.Y;

		if (Fields.Find(NeighborCoord))
		{
			//Take action on field around
			CheckField(Cast<UFieldStaticMeshComponent>(*Fields.Find(NeighborCoord)));
		}
	}
}

void AGrid::CheckField(UFieldStaticMeshComponent* FieldToCheck)
{
	if (FieldToCheck->bRevealed)
	{
		return;
	}
	else if (FieldToCheck->bMined)
	{
		FieldToCheck->Reveal();

	}
	else if (FieldToCheck->NeightbourMinesCounter > 0)
	{
		FieldToCheck->Reveal();
	}
	else if (FieldToCheck->NeightbourMinesCounter == 0)
	{
		FieldToCheck->Reveal();
		CheckFieldNeighbors(FieldToCheck);
	}
}


void AGrid::CheckFlag(UFieldStaticMeshComponent* FieldToCheck)
{
	if (FieldToCheck->bRevealed)
	{
		return;
	}

	if (FieldToCheck->NeightbourMinesCounter > 0 && FieldToCheck->bRevealed)
	{
		return;
	}

	if (!FieldToCheck->bFlaged)
	{
		FieldToCheck->ShowFlag();
	}
	else if (FieldToCheck->bFlaged)
	{
		FieldToCheck->TakeFlag();
	}
}

void AGrid::SetGridSize(int NewGridSize)
{
	GridSize = FMath::Clamp(NewGridSize, 4, MaxGridSize);
}

void AGrid::ResetGrid()
{
	Cast<AMinesweeperPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->bGameStarted = false;

	FCoordinates NewCoordinates;
	NewCoordinates.X = 0;
	NewCoordinates.Y = 0;
	for (const TPair<FCoordinates, UFieldStaticMeshComponent*> &Field : Fields)
	{ 
		Field.Value->ResetField();
		Field.Value->SetStaticMesh(nullptr);
		Field.Value->SetCoordinates(NewCoordinates);
	}

	Fields.Empty();
}


void AGrid::SetMinesNumber(int NewMinesNumber)
{
	MinesNumber = FMath::Clamp(NewMinesNumber, 0, (GridSize * GridSize - 1));
}


void AGrid::CheckFieldByCoordinates(FCoordinates Coordinates)
{
	CheckField(Cast<UFieldStaticMeshComponent>(*Fields.Find(Coordinates)));
}


//FString projectDir = FPaths::GameDir();
//projectDir += "/Content/data/events.json";
////
//
//if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*projectDir))
//{
//	UE_LOG(ShimLog, Warning, TEXT("FILE NOT FOUND %s"), *projectDir);
//	return;
//}

//
//FString FileData = "TEST";
//FFileHelper::LoadFileToString(FileData, *projectDir);
//TSharedPtr<FJsonObject> JsonParsed;
//
//TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(FileData);
//
//if (FJsonSerializer::Deserialize(JsonReader, JsonParsed))
//{
//	TArray <TSharedPtr<FJsonValue>> zonesJs = JsonParsed->GetArrayField("Zones");
//	for (int itZones = 0; itZones != zonesJs.Num(); itZones++) 
//	{
//		TSharedPtr<FJsonObject> temp = zonesJs[itZones]->AsObject();
//		TArray <TSharedPtr<FJsonValue>> eventJs = temp->GetArrayField("events");
//
//		for (int itEvents = 0; itEvents != eventJs.Num(); itEvents++) 
//		{
//			TSharedPtr<FJsonObject> tempEventJs = eventJs[itEvents]->AsObject();
//			FString nameEvents = tempEventJs->GetStringField("name");
//
//		}
//
//	}
//}