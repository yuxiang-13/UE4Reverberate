// Fill out your copyright notice in the Description page of Project Settings.


#include "DDObject/DDGameModeBase.h"




ADDGameModeBase::ADDGameModeBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADDGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	RegisterToModule(ModuleName, ObjectName, ClassName);
}
