// Fill out your copyright notice in the Description page of Project Settings.


#include "DDCore/DDCenterModule.h"

#include "DDObject/DDOO.h"


void UDDCenterModule::IterChangeModuleType(UDDModule* Module, FName ModType)
{
	Module->ChangeModuleType(ModType);
	
	for (int i = 0; i < Module->GetAttachChildren().Num(); ++i)
	{
		UDDModule* ChildModule = Cast<UDDModule>(Module->GetAttachChildren()[i]);
		if (ChildModule)
		{
			// 初始化模组写的子模组，并且继续遍历
			Module->ChildrenModule.Push(ChildModule);
			IterChangeModuleType(ChildModule, ModType);
		}
	}
}

void UDDCenterModule::IterCreateManager(UDDModule* Module)
{
	Module->CreateManager();

	// 迭代
	for (int i = 0; i < Module->ChildrenModule.Num(); i++)
	{
		IterCreateManager(Module->ChildrenModule[i]);
	}
}

void UDDCenterModule::IterModuleInit(UDDModule* Module)
{
	Module->ModuleInit();

	// 迭代
	for (int i = 0; i < Module->ChildrenModule.Num(); i++)
	{
		IterModuleInit(Module->ChildrenModule[i]);
	}
}

void UDDCenterModule::IterModuleBeginPlay(UDDModule* Module)
{
	Module->ModuleBeginPlay();

	// 迭代
	for (int i = 0; i < Module->ChildrenModule.Num(); i++)
	{
		IterModuleBeginPlay(Module->ChildrenModule[i]);
	}
}

void UDDCenterModule::IterModuleTick(UDDModule* Module, float DeltaSeconds)
{
	Module->ModuleTick(DeltaSeconds);

	// 迭代
	for (int i = 0; i < Module->ChildrenModule.Num(); i++)
	{
		IterModuleTick(Module->ChildrenModule[i], DeltaSeconds);
	}
}

void UDDCenterModule::TotalGatherModule(FName ModType)
{
	if (ModType.IsNone())
	{
		return;
	}

	// 先获取所有模组到数组
	TArray<UDDModule*> GatherGroup;
	// 迭代添加到数组
	IterGatherModule(this, GatherGroup);

	// 获取枚举元素的数量
	int32 ModuleName = FindObject<UEnum>((UObject*)ANY_PACKAGE, *(ModType.ToString()), true)->GetMaxEnumValue();
	// 填充空对象到ModuleGroup
	for (int i = 0; i < ModuleName; ++i)
	{
		ModuleGroup.Push(NULL);
	}
	// 按照模组ID填充模组 到 ModuleGroup
	for (int i = 0; i < ModuleGroup.Num(); ++i)
	{
		ModuleGroup[GatherGroup[i]->ModuleIndex] = GatherGroup[i];
	}
}

void UDDCenterModule::IterGatherModule(UDDModule * Module, TArray<UDDModule*> & GatherGroup)
{
	GatherGroup.Push(Module);

	for (int i = 0; i < Module->ChildrenModule.Num(); ++i)
	{
		IterGatherModule(Module->ChildrenModule[i], GatherGroup);
	}
}

bool UDDCenterModule::RegisterToModule(IDDOO* ObjctInst)
{
	// 判断模组ID是否有效且进行过注册
	if (ObjctInst->GetModuleIndex() < ModuleGroup.Num() && ModuleGroup[ObjctInst->GetModuleIndex()])
	{
		ModuleGroup[ObjctInst->GetModuleIndex()]->RegisterObject(ObjctInst);
		return true;
	}
	return false;
}
