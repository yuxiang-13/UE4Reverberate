// Fill out your copyright notice in the Description page of Project Settings.


#include "DDObject/DDLevelScriptActor.h"


ADDLevelScriptActor::ADDLevelScriptActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADDLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

	//�Զ�ע��
	RegisterToModule(ModuleName, ObjectName, ClassName);
}