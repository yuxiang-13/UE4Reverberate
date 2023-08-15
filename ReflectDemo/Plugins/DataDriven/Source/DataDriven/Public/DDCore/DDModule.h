// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"

#include "DDMessage.h" // 1 消息模块

#include "DDModule.generated.h"

class UDDModel;// 2 模块
class UDDWealth;// 3 模块
class IDDOO;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DATADRIVEN_API UDDModule : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDDModule();

	void CreateManager();

	//Module的Init函数
	virtual void ModuleInit();

	//Module的BeginPlay函数
	virtual void ModuleBeginPlay();

	//Module的Tick函数
	virtual void ModuleTick(float DeltaSeconds);

	//设定模型枚举类型
	void ChangeModuleType(FName ModuleType);

	//注册对象到数据模块
	void RegisterObject(IDDOO* ObjectInst);

	//模组所属对象销毁自身
	void ChildDestroy(FName ObjectName);

	//反射事件批量销毁对象
	UFUNCTION()
		void DestroyObject(EAgreementType Agreement, TArray<FName> TargetNameGroup);

	//反射事件批量激活对象
	UFUNCTION()
		void EnableObject(EAgreementType Agreement, TArray<FName> TargetNameGroup);

	//反射事件批量失活对象
	UFUNCTION()
		void DisableObject(EAgreementType Agreement, TArray<FName> TargetNameGroup);

	//调用模组方法
	void ExecuteFunction(DDModuleAgreement Agreement, DDParam* Param);

	//调用对象方法
	void ExecuteFunction(DDObjectAgreement Agreement, DDParam* Param);

	//注册调用接口
	template<typename RetType, typename... VarTypes>
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


	
	// 外部方法，单纯获取资源链接
	FWealthURL* GainWealthURL(FName WealthName);
	// 获取一种了类型的资源URL
	void GainWealthURL(FName WealthKind, TArray<FWealthURL*>& OutURL);
	
	// 加载Object类型资源接口
	void LoadObjectWealth(FName WealthName, FName ObjectName, FName FunName);
	void LoadObjectWealthKind(FName WealthKind, FName ObjectName, FName FunName);

	
	// 加载Class类型资源接口
	void LoadClassWealth(FName WealthName, FName ObjectName, FName FunName);
	void LoadClassWealthKind(FName WealthKind, FName ObjectName, FName FunName);
	
	// 创建一个对象实例
	void BuildSingleClassWealth(EWealthType WealthType, FName WealthName, FName ObjectName, FName FunName, FTransform SpawnTransform);
	
	// 创建同资源种类名的对象实例，同种类名下的每个资源链接创建一个对象实例
	void BuildKindClassWealth(EWealthType WealthType, FName WealthKind, FName ObjectName, FName FunName, TArray<FTransform> SpawnTransforms);
public:
	UPROPERTY(EditAnywhere, Category="DataDriven")
	TArray<UWealthData*> WealthData;
	
	TArray<UDDModule*> ChildrenModule;
	
	int32 ModuleIndex;
protected:

	//执行单个对象方法
	void ExecuteSelfObject(DDObjectAgreement Agreement, DDParam* Param);
	//执行其余对象的方法
	void ExecuteOtherObject(DDObjectAgreement Agreement, DDParam* Param);
	//支持相同类的区域对象方法
	void ExecuteClassOtherObject(DDObjectAgreement Agreement, DDParam* Param);
	//执行类对象的方法
	void ExecuteSelfClass(DDObjectAgreement Agreement, DDParam* Param);
	//执行其他类对象的方法
	void ExecuteOtherClass(DDObjectAgreement Agreement, DDParam* Param);
	//执行所有对象的方法
	void ExecuteAll(DDObjectAgreement Agreement, DDParam* Param);

protected:

	//数据模块指针
	UDDModel* Model;
	//消息模块指针
	UDDMessage* Message;
	//资源模块指针
	UDDWealth* Wealth;
};

template<typename RetType, typename... VarTypes>
DDCallHandle<RetType, VarTypes...>
UDDModule::RegisterCallPort(FName CallName)
{
	return Message->RegisterCallPort<RetType, VarTypes...>(CallName);
}

template<typename RetType, typename... VarTypes>
DDFunHandle UDDModule::RegisterFunPort(FName CallName, TFunction<RetType(VarTypes...)> InsFun)
{
	return Message->RegisterFunPort<RetType, VarTypes...>(CallName, InsFun);
}

template <class UserClass>
FInputAxisBinding& UDDModule::DDBindAxis(UserClass* UserObj,
	typename FInputAxisHandlerSignature::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, const FName AxisName_1)
{
	return Message->DDBindAxis(UserObj, InMethod, AxisName_1);
}

template <class UserClass>
FInputTouchBinding& UDDModule::DDBindTouch(UserClass* UserObj,
	typename FInputTouchHandlerSignature::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod,
	const EInputEvent KetEvent)
{
	return Message->DDBindTouch(UserObj, InMethod, KetEvent);
}

template <class UserClass>
FInputKeyBinding& UDDModule::DDBindInput(UserClass* UserObj,
	typename FInputActionHandlerSignature::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, const FKey Key,
	const EInputEvent KetEvent)
{
	return Message->DDBindInput(UserObj, InMethod, Key, KetEvent);
}

template <class UserClass>
FInputActionBinding& UDDModule::DDBindAction(UserClass* UserObj,
	typename FInputActionHandlerSignature::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod,
	const FName ActionName, const EInputEvent KetEvent)
{
	return Message->DDBindAction(UserObj, InMethod, ActionName, KetEvent);
}

template <typename UserClass>
UDDInputBinder& UDDModule::DDBindInput(UserClass* UserObj,
	typename FDDInputEvent::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, TArray<FKey>& KeyGroup,
	FName ObjectName)
{
	return Message->DDBindInput(UserObj, InMethod, KeyGroup, ObjectName);
}
