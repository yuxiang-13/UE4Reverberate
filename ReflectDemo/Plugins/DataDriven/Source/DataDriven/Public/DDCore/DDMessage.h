// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	
	// 模块生命周期函数
	virtual void MessageInit();
	virtual void MessageBeginPlay();
	virtual void MessageTick(float DeltaSeconds);

	// 注册调用接口
	template<typename RetType, typename ... VarTypes>
	DDCallHandle<RetType, VarTypes ...> RegisterCallPort(FName CallName);
	// 注册方法接口
	template<typename RetType, typename... VarTypes>
	DDFunHandle RegisterFunPort(FName CallName, TFunction<RetType(VarTypes...)> InsFun);

	// 开始一个协程，返回true表示开启成功，false表示已经投同对象名同协程任务名，的协程
	bool StartCoroution(FName ObjectName, FName CoroName, DDCoroTask* CoroTask);
	// 停止一个协程,返回true表示停止成功，返回false表示携程不存在了
	bool StopCoroution(FName ObjectName, FName CoroName);
	// 停止所有的协程
	void StopAllCorotion(FName ObjectName);
protected:
	// 事件队列
	DDMsgQuene* MsgQuene;

	// 协程序列 Key=>对象名  Value的Key=>协程任务的名字
	TMap<FName, TMap<FName, DDCoroTask*>> CoroStack;
};

template <typename RetType, typename ... VarTypes>
DDCallHandle<RetType, VarTypes...> UDDMessage::RegisterCallPort(FName CallName)
{
	return MsgQuene->RegisterCallPort<RetType, VarTypes ...>(CallName);
}

template<typename RetType, typename... VarTypes>
DDFunHandle UDDMessage::RegisterFunPort(FName CallName, TFunction<RetType(VarTypes...)> InsFun)
{
	return MsgQuene->RegisterFunPort<RetType, VarTypes...>(CallName, InsFun);
}
