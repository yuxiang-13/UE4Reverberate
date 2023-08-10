// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DDMM.h"
#include "DDMessage.generated.h"

/**
 * 
 */
UCLASS()
class DATADRIVEN_API UDDMessage : public UObject, public IDDMM
{
	GENERATED_BODY()

public:

	UDDMessage();

	virtual void MessageInit();

	virtual void MessageBeginPlay();

	virtual void MessageTick(float DeltaSeconds);

	//注册调用接口
	template<typename RetType,typename... VarTypes>
	DDCallHandle<RetType, VarTypes...> RegisterCallPort(FName CallName);

	//注册方法接口
	template<typename RetType, typename... VarTypes>
	DDFunHandle RegisterFunPort(FName CallName, TFunction<RetType(VarTypes...)> InsFun);

	//开启一个协程, 返回true说明开启成功, 返回false说明已经有同对象名同协程任务名的协程存在
	bool StartCoroutine(FName ObjectName, FName CoroName, DDCoroTask* CoroTask);
	//停止一个协程, 返回true说明停止协程成功, 返回false说明协程早已经停止
	bool StopCoroutine(FName ObjectName, FName CoroName);
	//停止该对象的所有协程
	void StopAllCorotine(FName ObjectName);

	// 开始一个延时方法，返回true说明成功，返回false说明已经存在
	bool StartInvoke(FName ObjectName, FName InvokeName, DDInvokeTask* InvokeTask);
	// 停止一个延时
	bool StopInvoke(FName ObjectName, FName InvokeName);
	// 停止某一个对象下的所有延时方法
	void StopAllInvoke(FName ObjectName);
protected:

	//事件队列
	DDMsgQuene* MsgQuene;

	//协程序列, 键1保存对象名, 值的键FName对应的是协程任务的名字
	TMap<FName, TMap<FName, DDCoroTask*>> CoroStack;

	// 延时序列
	TMap<FName, TMap<FName, DDInvokeTask*>> InvokeStack;
};

template<typename RetType, typename... VarTypes>
DDCallHandle<RetType, VarTypes...>
UDDMessage::RegisterCallPort(FName CallName)
{
	return MsgQuene->RegisterCallPort<RetType, VarTypes...>(CallName);
}

template<typename RetType, typename... VarTypes>
DDFunHandle UDDMessage::RegisterFunPort(FName CallName, TFunction<RetType(VarTypes...)> InsFun)
{
	return MsgQuene->RegisterFunPort<RetType, VarTypes...>(CallName, InsFun);
}
