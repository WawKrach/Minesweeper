// Fill out your copyright notice in the Description page of Project Settings.


#include "MinesweeperPlayerController.h"
#include "Grid.h"


void AMinesweeperPlayerController::BeginPlay()
{
	Super::BeginPlay();
	GameMode = (AMinesweeperGameModeBase*)UGameplayStatics::GetGameMode(GetWorld());
}


void AMinesweeperPlayerController::SetupInputComponent()
{
	// Always call this.
	Super::SetupInputComponent();

	InputComponent->BindAction("LeftClick", IE_Pressed, this, &AMinesweeperPlayerController::ClickLeft);
	InputComponent->BindAction("RightClick", IE_Pressed, this, &AMinesweeperPlayerController::ClickRight);
}


void AMinesweeperPlayerController::ClickLeft()
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECollisionChannel::ECC_WorldDynamic, true, Hit);
	if (Hit.bBlockingHit)
	{
		AGrid* ClickedGrid = Cast<AGrid>(Hit.Actor);
		if (ClickedGrid)
		{
			UFieldStaticMeshComponent* SelectedField = Cast<UFieldStaticMeshComponent>(Hit.Component);
			if (SelectedField)
			{
				if (!bGameStarted)
				{
					bGameStarted = true;
					GameMode->StartGame(SelectedField->Coordinates);
				}
				//GameMode->RevealField(SelectedField->Coordinates);//To przerzuciæ na koniec do recive message po przyjêciu rozpoeczêcia rozgrywki
			}
		}
	}
}


void AMinesweeperPlayerController::ClickRight()
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECollisionChannel::ECC_WorldDynamic, true, Hit);
	if (Hit.bBlockingHit)
	{
		AGrid* ClickedGrid = Cast<AGrid>(Hit.Actor);
		if (ClickedGrid)
		{
			UFieldStaticMeshComponent* SelectedField = Cast<UFieldStaticMeshComponent>(Hit.Component);
			if (SelectedField)
			{
				ClickedGrid->CheckFlag(SelectedField);
			}
		}
	}
}