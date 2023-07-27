// Fill out your copyright notice in the Description page of Project Settings.


#include "DDObject/DDPlayerController.h"


ADDPlayerController::ADDPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADDPlayerController::BeginPlay()
{
	Super::BeginPlay();

	RegisterToModule(ModuleName, ObjectName, ClassName);
}