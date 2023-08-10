// Fill out your copyright notice in the Description page of Project Settings.


#include "DDObject/DDOO.h"

#include "DDCore/DDDriver.h"

// Add default functionality here for any IDDOO functions that are not pure virtual.

void IDDOO::RegisterToModule(FName ModName, FName ObjName /*= FName()*/, FName ClsName /*= FName()*/)
{
	//判断是否已经注册到框架了
	if (IDriver && IModule)
		return;
	//如果模组名为空, 直接返回
	if (ModName.IsNone())
		return;
	//指定对象名和类名
	if (!ObjName.IsNone())
		IObjectName = ObjName;
	if (!ClsName.IsNone())
		IClassName = ClsName;
	//获取UObject主体
	IBody = Cast<UObject>(this);
	//获取驱动器
	IDriver = UDDCommon::Get()->GetDriver();
	//注册到模组
	if (IDriver)
	{
		//如果获得的ID为负直接返回
		ModuleIndex = DDH::GetEnumIndexFromName(IDriver->ModuleType.ToString(), ModName);
		if (ModuleIndex < 0)
		{
			DDH::Debug(10) << GetObjectName() << " Get " << ModName << " ModuleIndex Failed!" << DDH::Endl();
			return;
		}
		//如果注册不成功说明还没有实例化对应的Module
		if (!IDriver->RegisterToModule(this))
			DDH::Debug(10) << GetObjectName() << " Register To " << ModName << " Failed !" << DDH::Endl();
	}
	else
		//DDriver不存在
		DDH::Debug(10) << GetObjectName() << " Get DDDriver Failed !" << DDH::Endl();
}

void IDDOO::RegisterToModule(int32 ModIndex, FName ObjName /*= FName()*/, FName ClsName /*= FName()*/)
{
	//判断是否已经注册到框架了
	if (IDriver && IModule)
		return;
	//指定对象名和类名
	if (!ObjName.IsNone())
		IObjectName = ObjName;
	if (!ClsName.IsNone())
		IClassName = ClsName;
	//获取UObject主体
	IBody = Cast<UObject>(this);
	//获取驱动器
	IDriver = UDDCommon::Get()->GetDriver();
	//注册到模组
	if (IDriver)
	{
		//如果获得的ID为负直接返回
		ModuleIndex = ModIndex;
		if (ModuleIndex < 0)
		{
			DDH::Debug(10) << GetObjectName() << " Get ModuleIndex " << ModuleIndex << " ModuleIndex Failed!" << DDH::Endl();
			return;
		}
		//如果注册不成功说明还没有实例化对应的Module
		if (!IDriver->RegisterToModule(this))
			DDH::Debug(10) << GetObjectName() << " Register To ModuleIndex " << ModuleIndex << " Failed !" << DDH::Endl();
	}
	else
		//DDriver不存在
		DDH::Debug(10) << GetObjectName() << " Get DDDriver Failed !" << DDH::Endl();
}

FName IDDOO::GetObjectName()
{
	if (!IObjectName.IsNone())
		return IObjectName;
	IObjectName = IBody->GetFName();
	return IObjectName;
}

FName IDDOO::GetClassName()
{
	if (!IClassName.IsNone())
		return IClassName;
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
		return IDriver->GetWorld();
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
		DDInit();
		LifeState = EBaseObjectLife::Init;
		break;
	case EBaseObjectLife::Init:
		DDLoading();
		LifeState = EBaseObjectLife::Loading;
		break;
	case EBaseObjectLife::Loading:
		DDRegister();
		LifeState = EBaseObjectLife::Register;
		break;
	case EBaseObjectLife::Register:
		DDEnable();
		LifeState = EBaseObjectLife::Enable;
		//设置运行状态为稳定
		RunState = EBaseObjectState::Stable;
		//返回true, 停止运行激活状态函数
		return true;
	}
	return false;
}

bool IDDOO::DestroyLife()
{
	switch (LifeState)
	{
	case EBaseObjectLife::Enable:
		DDDisable();
		LifeState = EBaseObjectLife::Disable;
		//设置状态为销毁
		RunState = EBaseObjectState::Destroy;
		break;
	case EBaseObjectLife::Disable:
		DDUnRegister();
		LifeState = EBaseObjectLife::UnRegister;
		//设置运行状态为销毁,避免从Disable状态下运行的对象没有修改RunState为销毁
		RunState = EBaseObjectState::Destroy;
		break;
	case EBaseObjectLife::UnRegister:
		DDUnLoading();
		LifeState = EBaseObjectLife::UnLoading;
		//返回true, 停止运行销毁状态函数
		return true;
	}
	return false;
}

void IDDOO::DDInit() {}

void IDDOO::DDLoading() {}

void IDDOO::DDRegister() {}

void IDDOO::DDEnable() {}

void IDDOO::DDTick(float DeltaSeconds) {}

void IDDOO::DDDisable() {}

void IDDOO::DDUnRegister() {}

void IDDOO::DDUnLoading() {}

void IDDOO::DDRelease() {}

void IDDOO::OnEnable()
{
	//设置状态为激活状态
	LifeState = EBaseObjectLife::Enable;
}

void IDDOO::OnDisable()
{
	//设置状态为失活状态
	LifeState = EBaseObjectLife::Disable;
}

void IDDOO::DDDestroy()
{
	IModule->ChildDestroy(GetObjectName());
}

void IDDOO::ExecuteFunction(DDModuleAgreement Agreement, DDParam* Param)
{
	if (Agreement.ModuleIndex == ModuleIndex)
		IModule->ExecuteFunction(Agreement, Param);
	else
		IDriver->ExecuteFunction(Agreement, Param);
}

void IDDOO::ExecuteFunction(DDObjectAgreement Agreement, DDParam* Param)
{
	if (Agreement.ModuleIndex == ModuleIndex)
		IModule->ExecuteFunction(Agreement, Param);
	else
		IDriver->ExecuteFunction(Agreement, Param);
}

bool IDDOO::StartCoroutine(FName CoroName, DDCoroTask* CoroTask)
{
	return IModule->StartCoroutine(GetObjectName(), CoroName, CoroTask);
}

bool IDDOO::StopCoroutine(FName CoroName)
{
	return IModule->StopCoroutine(GetObjectName(), CoroName);
}

void IDDOO::StopAllCorotine()
{
	return IModule->StopAllCorotine(GetObjectName());
}
