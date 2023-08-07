// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DDCore/DDModule.h"
#include "DDCenterModule.generated.h"

class IDDOO;
/**
 * 
 */
UCLASS()
class DATADRIVEN_API UDDCenterModule : public UDDModule
{
	GENERATED_BODY()
public:
	// 迭代调用本模组以及子模组的EditChangeModuleType方法
	void IterChangeModuleType(UDDModule* Module, FName ModType);
	
	// 递归初始化 子模组
	void IterCreateManager(UDDModule* Module);

	void IterModuleInit(UDDModule* Module);

	void IterModuleBeginPlay(UDDModule* Module);

	void IterModuleTick(UDDModule* Module, float DeltaSeconds);

	// 收集 模组
	void TotalGatherModule(FName ModType);
	// 提取所有模组到数组
	void IterGatherModule(UDDModule* Module, TArray<UDDModule*> & GatherGroup);

	// 注册对象到 模组
	bool RegisterToModule(IDDOO* ObjctInst);

	// 中心执行反射方法
	void AllotExecuteFunction(DDModuleAgreement Agreement, DDParam* Param);

	// 中心执行反射方法
	void AllotExecuteFunction(DDObjectAgreement Agreement, DDParam* Param);

	
	// 注册方法接口
	// 特殊在，需要传 模组ID， 来确定指定要注册方法到指定模组下的 事件节点
	template<typename RetType, typename... VarTypes>
	DDFunHandle AllotRegisterFunPort(int32 ModuleID, FName CallName, TFunction<RetType(VarTypes...)> InsFun);
protected:
	// 中心模组保存 模组数组，顺序与枚举相同
	UPROPERTY()
	TArray<UDDModule*> ModuleGroup;
	
};

template <typename RetType, typename ... VarTypes>
DDFunHandle UDDCenterModule::AllotRegisterFunPort(int32 ModuleID, FName CallName, TFunction<RetType(VarTypes...)> InsFun)
{
	if (ModuleGroup[ModuleID])
	{
		return ModuleGroup[ModuleID]->RegisterFunPort<RetType, VarTypes...>(CallName, InsFun);
	} else
	{
		// 返回没有任何的方法句柄
		return DDFunHandle();
	}
}
