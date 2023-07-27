// Fill out your copyright notice in the Description page of Project Settings.


#include "DDObject/DDActor.h"

// Sets default values
ADDActor::ADDActor()
{

}

void ADDActor::DDRelease()
{
	IDDOO::DDRelease();
	//能调用这个方法那么一定是注册到了框架,获取的世界一定不为空
	GetDDWorld()->DestroyActor(this);
}

void ADDActor::BeginPlay()
{
	Super::BeginPlay();

	// 自动注册到框架
	RegisterToModule(ModuleName, ObjectName, ClassName);
}

