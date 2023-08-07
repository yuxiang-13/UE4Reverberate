// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DDCommon/DDCommon.h"

#include "DDMM.generated.h"

class UDDModule;


UINTERFACE(MinimalAPI)
class UDDMM : public UInterface
{
	GENERATED_BODY()
};

class DATADRIVEN_API IDDMM
{
	GENERATED_BODY()

public:
	// 指定 所属模组
	void AssignModule(UDDModule* Mod);
protected:
	// 执行反射方法
	void ExecuteFunction(DDModuleAgreement Agreement, DDParam* Param);
	
	// 执行反射方法
	void ExecuteFunction(DDObjectAgreement Agreement, DDParam* Param);
protected:
	// 保存对应的模组
	UDDModule* IModule;

	// 保存驱动器
	ADDDriver* IDriver;
	
	// 对应模组的序号
	int32 ModuleIndex;
};
