// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "MinesweeperGameModeBase.h"
#include "MinesweeperPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MINESWEEPER_API AMinesweeperPlayerController : public APlayerController
{
	GENERATED_BODY()
	

public:

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	 void ClickLeft(); 
	 void ClickRight();

	 bool bGameStarted;

	 AMinesweeperGameModeBase* GameMode;
};
