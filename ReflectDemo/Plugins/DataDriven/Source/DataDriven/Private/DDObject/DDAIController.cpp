// Fill out your copyright notice in the Description page of Project Settings.


#include "DDObject/DDAIController.h"

ADDAIController::ADDAIController()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADDAIController::BeginPlay()
{
	Super::BeginPlay();

	//自动注册
	RegisterToModule(ModuleName, ObjectName, ClassName);
}
