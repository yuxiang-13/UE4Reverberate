// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFrame/FWGameMode.h"

#include "Common/FWDataSingleton.h"

AFWGameMode::AFWGameMode()
{
}

void AFWGameMode::BeginPlay()
{
	Super::BeginPlay();

	UFWDataSingleton* DataSingleton = Cast<UFWDataSingleton>(GEngine->GameSingleton);
	//FWHelper::Debug(DataSingleton->SourceVector.ToString(), 500.f);
}
