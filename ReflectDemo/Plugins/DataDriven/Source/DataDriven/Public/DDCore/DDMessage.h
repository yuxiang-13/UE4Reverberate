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
protected:
	// 事件队列
	DDMsgQuene* MsgQuene;
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
