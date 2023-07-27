// Fill out your copyright notice in the Description page of Project Settings.


#include "DDObject/DDActorComponent.h"

UDDActorComponent::UDDActorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// 对组件设置， 设置允许销毁
	bAllowAnyoneToDestroyMe = true;
}

void UDDActorComponent::BeginPlay()
{
	Super::BeginPlay();

	//自动注册
	RegisterToModule(ModuleName, ObjectName, ClassName);

}

void UDDActorComponent::DDRelease()
{
	IDDOO::DDRelease();
	//从组件中删除自己,并标记为准备被gc回收
	DestroyComponent();
}





