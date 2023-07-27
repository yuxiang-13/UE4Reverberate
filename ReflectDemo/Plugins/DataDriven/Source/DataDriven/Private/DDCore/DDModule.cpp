// Fill out your copyright notice in the Description page of Project Settings.


#include "DDCore/DDModule.h"

#include "DDCore/DDModel.h"
#include "DDCore/DDWealth.h"
#include "DDObject/DDOO.h"

// Sets default values for this component's properties
UDDModule::UDDModule()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UDDModule::CreateManager()
{
	// 创建三个模块   自定义的UObject类，NewObject<UDDModel>() 参数不用传This
	
	// 数据模块 指针
	Model = NewObject<UDDModel>();
	// 消息模块 指针
	Message = NewObject<UDDMessage>();
	// 资源模块 指针
	Wealth = NewObject<UDDWealth>();

	// 避免被垃圾回收销毁
	Model->AddToRoot();
	Message->AddToRoot();
	Wealth->AddToRoot();

	// 指定模组
	Model->AssignModule(this);
	Message->AssignModule(this);
	Wealth->AssignModule(this);
}

void UDDModule::ModuleInit()
{
	Model->ModelInit();
	Message->MessageInit();
	Wealth->WealthInit();

}

void UDDModule::ModuleBeginPlay()
{
	Model->ModelBeginPlay();
	Message->MessageBeginPlay();
	Wealth->WealthBeginPlay();
}

void UDDModule::ModuleTick(float DeltaSeconds)
{
	Model->ModelTick(DeltaSeconds);
	Message->MessageTick(DeltaSeconds);
	Wealth->WealthTick(DeltaSeconds);
}

void UDDModule::ChangeModuleType(FName ModuleType)
{
	ModuleIndex = DDH::GetEnumIndexFromName(ModuleType.ToString(), GetFName());

	if (ModuleIndex < 0)
	{
		DDH::Debug(10) << "生成模具错误----" << GetName() << DDH::Endl();
	}
}

void UDDModule::RegisterObject(IDDOO* ObjectInst)
{
	// 注册对象到数据模块
	Model->RegisterObject(ObjectInst);
	// 把此 模组自己  注册到对象数组
	ObjectInst->AssignModule(this);
}

void UDDModule::ChildDestroy(FName ObjectName)
{
	Model->DestroyObject(ObjectName);
}

