// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DDMM.h"
#include "DDMessage.generated.h"


#pragma region InputBinder

// 声明个委托
DECLARE_DELEGATE(FDDInputEvent);

UCLASS() // 不要拉下 DATADRIVEN_API  ***！！！*** 为什么声明UCLass？ 因为要支持反射，而且只有继承自 -->UObject的类，才能让委托方法BindUObject去绑定
class DATADRIVEN_API UDDInputBinder : public UObject
{
	GENERATED_BODY()
public:
	UDDInputBinder();
	void PressEvent();
	void ReleaseEvent();
public:
	// 按下+1 送开-1
	uint8 InputCount;
	// 绑定着的按键数量
	uint8 TotalCount;
	// 游戏暂停时也能触发
	uint8 bExecuteWhenPause;

	FDDInputEvent InputDele;
public: // *****************！！！*********** uint8  ===  最大255个 按键绑定
	template<class UserClass>
	void InitBinder(UserClass* UserObj, typename FDDInputEvent::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, uint8 InCount)
	
	{
		TotalCount = InCount;
		InputDele.BindUObject(UserObj, InMethod);
	}
};

#pragma endregion 

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

	// 绑定Axis按键事件 InputAxis
	template<class UserClass>
	FInputAxisBinding& DDBindAxis(UserClass* UserObj, typename FInputAxisHandlerSignature::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, const FName AxisName_1);

	// 绑定触摸事件 InputTouch
	template<class UserClass>
	FInputTouchBinding& DDBindTouch(UserClass* UserObj, typename FInputTouchHandlerSignature::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, const EInputEvent KetEvent);

	// 绑定单个按键事件 InputAction
	template<class UserClass>
	FInputKeyBinding& DDBindInput(UserClass* UserObj, typename FInputActionHandlerSignature::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, const FKey Key, const EInputEvent KetEvent);
	
	// 绑定Action按键事件 InputAction
	template<class UserClass>
	FInputActionBinding& DDBindAction(UserClass* UserObj, typename FInputActionHandlerSignature::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, const FName ActionName, const EInputEvent KetEvent);

	// 绑定多个按键
	template<typename UserClass>
	UDDInputBinder& DDBindInput(UserClass* UserObj, typename FDDInputEvent::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, TArray<FKey>& KeyGroup, FName ObjectName);;

	// 解绑对象的所有按键事件
	void UnBindInput(FName ObjectName);
protected:

	//事件队列
	DDMsgQuene* MsgQuene;

	//协程序列, 键1保存对象名, 值的键FName对应的是协程任务的名字
	TMap<FName, TMap<FName, DDCoroTask*>> CoroStack;

	// 延时序列
	TMap<FName, TMap<FName, DDInvokeTask*>> InvokeStack;
	
	// PlaterController 指针
	APlayerController* PlayerController;

	// 绑定案件事件序列
	TMap<FName, TArray<UDDInputBinder*>> BinderGroup;
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

template <class UserClass>
FInputAxisBinding& UDDMessage::DDBindAxis(UserClass* UserObj,
	typename FInputAxisHandlerSignature::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, const FName AxisName_1)
{
	return PlayerController->InputComponent->BindAxis(AxisName_1, UserObj, InMethod);
}

template <class UserClass>
FInputTouchBinding& UDDMessage::DDBindTouch(UserClass* UserObj,
	typename FInputTouchHandlerSignature::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod,
	const EInputEvent KetEvent)
{
	return PlayerController->InputComponent->BindTouch(KetEvent, UserObj, InMethod);
}

template <class UserClass>
FInputKeyBinding& UDDMessage::DDBindInput(UserClass* UserObj,
	typename FInputActionHandlerSignature::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, const FKey Key,
	const EInputEvent KetEvent)
{
	return PlayerController->InputComponent->BindKey(Key, KetEvent, UserObj, InMethod);
}

template <class UserClass>
FInputActionBinding& UDDMessage::DDBindAction(UserClass* UserObj,
	typename FInputActionHandlerSignature::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod,
	const FName ActionName, const EInputEvent KetEvent)
{
	return PlayerController->InputComponent->BindAction(ActionName, KetEvent, UserObj, InMethod);
}

template <typename UserClass>
UDDInputBinder& UDDMessage::DDBindInput(UserClass* UserObj,
	typename FDDInputEvent::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, TArray<FKey>& KeyGroup,
	FName ObjectName)
{
	UDDInputBinder* InputBinder = NewObject<UDDInputBinder>();
	InputBinder->InitBinder(UserObj, InMethod, KeyGroup.Num());
	// 不要让垃圾回收掉
	InputBinder->AddToRoot();
	for (int i = 0; i < KeyGroup.Num(); ++i)
	{
		// BindKey()返回的是 FInputKeyBinding
		// PlayerController->InputComponent->BindKey(KeyGroup[i], IE_Pressed, InputBinder, &UDDInputBinder::PressEvent).bExecuteWhenPaused = true;
		FInputKeyBinding InputKeyBind_1 = PlayerController->InputComponent->BindKey(KeyGroup[i], IE_Pressed, InputBinder, &UDDInputBinder::PressEvent);
		// 游戏暂停也会执行
		InputKeyBind_1.bExecuteWhenPaused = true;

		
		FInputKeyBinding InputKeyBind_2 = PlayerController->InputComponent->BindKey(KeyGroup[i], IE_Released, InputBinder, &UDDInputBinder::ReleaseEvent);
		// 游戏暂停也会执行
		InputKeyBind_2.bExecuteWhenPaused = true;
	}

	if (!BinderGroup.Contains(ObjectName))
	{
		TArray<UDDInputBinder*> BinderList;
		BinderGroup.Add(ObjectName, BinderList);
	}
	BinderGroup.Find(ObjectName)->Push(InputBinder);
	return *InputBinder;
}
