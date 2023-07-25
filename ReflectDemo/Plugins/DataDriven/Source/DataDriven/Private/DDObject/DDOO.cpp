// Fill out your copyright notice in the Description page of Project Settings.


#include "DDObject/DDOO.h"

#include "DDCore/DDDriver.h"

// Add default functionality here for any IDDOO functions that are not pure virtual.
void IDDOO::RegisterToModule(FName ModName, FName ObjName, FName ClsName)
{
	// 判断是否已经注册到 框架
	if (IDriver && IModule)
	{
		return;
	}

	// 如果模组名为空，直接返回
	if (ModName.IsNone())
	{
		return;
	}

	// 指定对象名和类名
	if (!ObjName.IsNone())
	{
		IObjectName = ObjName;
	}
	if (!ClsName.IsNone())
	{
		IClassName = ClsName;
	}

	// 获取UObject 主体
	IBody = Cast<UObject>(this);
	// 获取驱动器
	IDriver = UDDCommon::Get()->GetDriver();
	// 注册到模组
	if (IDriver)
	{
		// 如果获取的ID为负 直接返回
		ModuleIndex = DDH::GetEnumIndexFromName(IDriver->ModuleType.ToString(), ModName);
		if (ModuleIndex < 0)
		{
			DDH::Debug(10) << GetObjectName() << "Get " << ModName << "  ModuleIndex   Failed!" << DDH::Endl();
			return;
		} else
		{
			// 开始把资源进行注册
			if (!IDriver->RegisterToModule(this))
			{
				DDH::Debug(10) << GetObjectName() << "Register to " << ModName << "  ModuleIndex   Failed!" << DDH::Endl();
			}
		}
	} else
	{
		// DDriver不存在
		DDH::Debug(10) << GetObjectName() << "Get " << ModName << "  ModuleIndex   Failed!" << DDH::Endl();
	}
}

FName IDDOO::GetObjectName()
{
	if (!IObjectName.IsNone())
	{
		return IObjectName;
	}
	IObjectName = IBody->GetFName();
	return IObjectName;
}

FName IDDOO::GetClassName()
{
	if (!IClassName.IsNone())
	{
		return IClassName;
	}
	IClassName = IBody->StaticClass()->GetFName();
	return IClassName;
}

int32 IDDOO::GetModuleIndex() const
{
	return ModuleIndex;
}

UObject* IDDOO::GetObjectBody() const
{
	return IBody;
}

void IDDOO::AssignModule(UDDModule* Mod)
{
	IModule = Mod;
}
