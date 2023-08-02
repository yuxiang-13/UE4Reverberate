// Fill out your copyright notice in the Description page of Project Settings.


#include "DDCore/DDMM.h"

#include "DDCore/DDDriver.h"
#include "DDCore/DDModule.h"

// Add default functionality here for any IDDMM functions that are not pure virtual.
void IDDMM::AssignModule(UDDModule* Mod)
{
	IModule = Mod;
	ModuleIndex = IModule->ModuleIndex;
	IDriver = UDDCommon::Get()->GetDriver();
}

void IDDMM::ExecuteFunction(DDModuleAgreement Agreement, DDParam* Param)
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

void IDDMM::ExecuteFunction(DDObjectAgreement Agreement, DDParam* Param)
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
