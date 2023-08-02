// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DDObject/DDActor.h"
#include "LifeCallActor.generated.h"

/**
 * 
 */
UCLASS()
class REFLECTDEMO_API ALifeCallActor : public ADDActor
{
	GENERATED_BODY()
public:
	virtual void DDInit() override;
	virtual void DDLoading() override;
	virtual void DDRegister() override;
	virtual void DDEnable() override;
	virtual void DDTick(float DeltaSeconds) override;

	
	virtual void DDDisable() override;//失活对象
	
	virtual void DDUnRegister() override;//注销数据或者事件
	
	virtual void DDUnLoading() override;//销毁绑定资源
	
	virtual void DDRelease() override;//释放自己
protected:
	float TimeCounter = 0;
	
protected:
	struct TestReflectParam : DDParam
	{
		struct
		{
			int32 Counter;
			FString InfoStr;
			bool BackResult;
		} Parameter;

		int32 Counter() {return Parameter.Counter;};
		FString InfoStr() {return Parameter.InfoStr;};
		bool BackResult() {return Parameter.BackResult;};

		// 构造，指定 参数指针
		TestReflectParam() { ParamPtr = &Parameter; };
	};

	void TestReflect(int32 InModuleIndex, FName FunctionName, int32 Counter, FString InfoStr, bool BackResult)
	{
		DDModuleAgreement Agreement;
		Agreement.ModuleIndex = InModuleIndex;
		Agreement.FunctionName = FunctionName;

		// 参数
		TestReflectParam* Param = new TestReflectParam();
		Param->Parameter.Counter = Counter;
		Param->Parameter.InfoStr = InfoStr;
		Param->Parameter.BackResult = BackResult;

		ExecuteFunction(Agreement, Param);
		// 销毁 参数指针
		delete Param;
	};

	// 返回参数指针
	TestReflectParam* TestReflectRT(int32 InModuleIndex, FName FunctionName, int32 Counter, FString InfoStr, bool BackResult)
	{
		DDModuleAgreement Agreement;
		Agreement.ModuleIndex = InModuleIndex;
		Agreement.FunctionName = FunctionName;

		// 参数
		TestReflectParam* Param = new TestReflectParam();
		Param->Parameter.Counter = Counter;
		Param->Parameter.InfoStr = InfoStr;
		Param->Parameter.BackResult = BackResult;

		ExecuteFunction(Agreement, Param);
		// 返回 参数指针
		return Param;
	}
};

