// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/ViewObject.h"

void UViewObject::DDEnable()
{
	Super::DDEnable();

	EChoSelfInfo(ModuleIndex, GetObjectName(), "EchoInfo");
}
