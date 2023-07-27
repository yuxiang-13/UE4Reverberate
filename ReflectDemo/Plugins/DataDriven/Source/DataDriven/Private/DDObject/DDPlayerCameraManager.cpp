// Fill out your copyright notice in the Description page of Project Settings.


#include "DDObject/DDPlayerCameraManager.h"



ADDPlayerCameraManager::ADDPlayerCameraManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADDPlayerCameraManager::BeginPlay()
{
	Super::BeginPlay();

	RegisterToModule(ModuleName, ObjectName, ClassName);
}
