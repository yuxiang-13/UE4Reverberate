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
			// DDH::Debug(10) << GetObjectName() << "Get " << ModName << "  ModuleIndex   Failed!" << DDH::Endl();
			return;
		} else
		{
			// 开始把资源进行注册
			if (!IDriver->RegisterToModule(this))
			{
				// // DDH::Debug(10) << GetObjectName() << "Register to " << ModName << "  ModuleIndex   Failed!" << DDH::Endl();
			}
		}
	} else
	{
		// DDriver不存在
		// DDH::Debug(10) << GetObjectName() << "Get " << ModName << "  ModuleIndex   Failed!" << DDH::Endl();
	}
}

void IDDOO::RegisterToModule(int32 ModIndex, FName ObjName, FName ClsName)
{
	// 判断是否已经注册到 框架
	if (IDriver && IModule)
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
		ModuleIndex = ModIndex;
		if (ModuleIndex < 0)
		{
			// DDH::Debug(10) << GetObjectName() << "Get ModuleIndex  " << ModuleIndex << "  ModuleIndex   Failed!" << DDH::Endl();
			return;
		} else
		{
			// 开始把资源进行注册
			if (!IDriver->RegisterToModule(this))
			{
				// DDH::Debug(10) << GetObjectName() << "Register to ModuleIndex  " << ModuleIndex << "  ModuleIndex   Failed!" << DDH::Endl();
			}
		}
	} else
	{
		// DDriver不存在
		// DDH::Debug(10) << GetObjectName() << "Get ModuleIndex " << ModuleIndex << "  ModuleIndex   Failed!" << DDH::Endl();
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
	IClassName = IBody->GetClass()->GetFName();
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

UWorld* IDDOO::GetDDWorld() const
{
	if (IDriver)
	{
		return IDriver->GetWorld();
	}
	return NULL;
}

void IDDOO::AssignModule(UDDModule* Mod)
{
	IModule = Mod;
}



bool IDDOO::ActiveLife()
{
	switch (LifeState)
	{
	case EBaseObjectLife::None:
		{
			DDInit();
			LifeState=EBaseObjectLife::Init;
		}
		break;
	case EBaseObjectLife::Init:
		{
			DDLoading();
			LifeState = EBaseObjectLife::Loading;
		}
		break;
	case EBaseObjectLife::Loading:
		{
			DDRegister();
			LifeState = EBaseObjectLife::Register;
		}
		break;
	case EBaseObjectLife::Register:
		{
			DDEnable();
			LifeState = EBaseObjectLife::Enable;
			// 设置运行状态为稳定状态
			RunState = EBaseObjectState::Stable;
			// 返回true 停止运行激活状态函数
			return true;
		}
		break;
	}
	return false;
}

bool IDDOO::DestroyLife()
{
	switch (LifeState)
	{
	case EBaseObjectLife::Enable:
		{
			DDDisable();
			LifeState = EBaseObjectLife::Disable;
			// 设置状态为 销毁
			RunState = EBaseObjectState::Destroy;
		}
		break;
	case EBaseObjectLife::Disable:
		{
			DDUnRegister();
			LifeState = EBaseObjectLife::UnRegister;

			// 设置运行状态为销毁，避免从Disable状态下运行的对象，没有修改RunState为销毁
			RunState = EBaseObjectState::Destroy;
		}
		break;
	case EBaseObjectLife::UnRegister:
		{
			DDUnLoading();
			LifeState = EBaseObjectLife::UnLoading;
		}
		// 返回true  停止运行销毁状态
		return true;
	}
	return false;
}

void IDDOO::OnEnable()
{
	// 设置状态 激活
	LifeState = EBaseObjectLife::Enable;
}

void IDDOO::OnDisable()
{
	//设置状态 失活
	LifeState = EBaseObjectLife::Disable;
}

void IDDOO::DDInit() {};

void IDDOO::DDLoading() {};

void IDDOO::DDRegister() {};

void IDDOO::DDEnable() {};

void IDDOO::DDTick(float DeltaSeconds) {};

void IDDOO::DDDisable() {};

void IDDOO::DDUnRegister() {};

void IDDOO::DDUnLoading() {};

void IDDOO::DDRelease() {};

void IDDOO::DDDestroy()
{
	IModule->ChildDestroy(GetObjectName());
}

void IDDOO::ExecuteFunction(DDModuleAgreement Agreement, DDParam* Param)
{
	if (Agreement.ModuleIndex == ModuleIndex)
	{
		IModule->ExecuteFunction(Agreement, Param);
	} else
	{
		// 查找对应模组的方法
		IDriver->ExecuteFunction(Agreement, Param);
	}
}

void IDDOO::ExecuteFunction(DDObjectAgreement Agreement, DDParam* Param)
{
	// 本模组  和  非本模组
	if (Agreement.ModuleIndex == ModuleIndex)
	{
		IModule->ExecuteFunction(Agreement, Param);
	} else
	{
		// 查找对应模组的方法
		IDriver->ExecuteFunction(Agreement, Param);
	}
};
