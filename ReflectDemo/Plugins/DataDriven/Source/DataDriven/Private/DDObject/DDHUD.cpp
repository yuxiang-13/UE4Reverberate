// Fill out your copyright notice in the Description page of Project Settings.


#include "DDObject/DDHUD.h"


ADDHUD::ADDHUD()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADDHUD::BeginPlay()
{
	Super::BeginPlay();

	//�Զ�ע��
	RegisterToModule(ModuleName, ObjectName, ClassName);
}