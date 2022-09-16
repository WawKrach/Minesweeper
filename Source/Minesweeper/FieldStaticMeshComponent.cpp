// Fill out your copyright notice in the Description page of Project Settings.


#include "FieldStaticMeshComponent.h"

UFieldStaticMeshComponent::UFieldStaticMeshComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UFieldStaticMeshComponent::CreateFieldMaterialInstance()
{
	FieldMaterial = UMaterialInstanceDynamic::Create(DefaultMaterial, this);
	SetMaterial(0, FieldMaterial);
}

void UFieldStaticMeshComponent::SetCoordinates(FCoordinates NewCoordinates)
{
	Coordinates = NewCoordinates;
}

void UFieldStaticMeshComponent::SetWorldLocationByCoordinates()
{
	FVector FieldWorldLocation;
	FieldWorldLocation.X = FieldSpace * Coordinates.X;
	FieldWorldLocation.Y = FieldSpace * Coordinates.Y;
	SetWorldLocation(FieldWorldLocation);
}


void UFieldStaticMeshComponent::AddNeighbourMine()
{
	NeightbourMinesCounter++;
}



void UFieldStaticMeshComponent::Reveal_Implementation()
{
	bRevealed = true;
	if (bFlaged)
	{
		TakeFlag();
	}
}


void UFieldStaticMeshComponent::ShowFlag_Implementation()
{
	bFlaged = true;
}


void UFieldStaticMeshComponent::TakeFlag_Implementation()
{
	bFlaged = false;
}


void UFieldStaticMeshComponent::ResetField_Implementation()
{
	bRevealed = false;
	bFlaged = false;
	NeightbourMinesCounter = 0;
	bMined = false;
	Coordinates.X = 0;
	Coordinates.Y = 0;
}
