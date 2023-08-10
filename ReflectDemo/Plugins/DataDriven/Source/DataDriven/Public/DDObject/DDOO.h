// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DDCore/DDDriver.h"
#include "DDCore/DDModule.h"
#include "UObject/Interface.h"
#include "DDOO.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDDOO : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class DATADRIVEN_API IDDOO
{
	GENERATED_BODY()

		// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//注册到框架
	void RegisterToModule(FName ModName, FName ObjName = FName(), FName ClsName = FName());

	//同上, 只转入模组Index
	void RegisterToModule(int32 ModIndex, FName ObjName = FName(), FName ClsName = FName());

	//获取对象名
	FName GetObjectName();

	//获取类名
	FName GetClassName();

	//获取模组Index
	int32 GetModuleIndex() const;

	//获取Obejct主体
	UObject* GetObjectBody() const;

	//框架获取世界函数
	UWorld* GetDDWorld() const;

	//从外部指定模组
	void AssignModule(UDDModule* Mod);

	//激活生命周期, 激活成功后返回true, 停止调用
	bool ActiveLife();

	//销毁生命周期函数, 销毁成功后返回true, 从数据模块注销
	bool DestroyLife();

	//生命周期,由模组管理器调用
	virtual void DDInit();//初始化
	virtual void DDLoading();//加载绑定的资源
	virtual void DDRegister();//注册数据或者事件
	virtual void DDEnable();//激活对象

	virtual void DDTick(float DeltaSeconds);//帧函数

	virtual void DDDisable();//失活对象
	virtual void DDUnRegister();//注销数据或者事件
	virtual void DDUnLoading();//销毁绑定资源
	virtual void DDRelease();//释放自己

	//激活对象
	virtual void OnEnable();

	//失活对象
	virtual void OnDisable();

	//销毁自己
	void DDDestroy();





public:

	//是否允许帧运行,如果要允许帧运行需要在构造函数或者BeginPlay设置,在UE4里默认为false
	bool IsAllowTickEvent;

	//生命周期状态
	EBaseObjectLife LifeState;

	//生命运行状态
	EBaseObjectState RunState;

protected:

	//执行反射方法
	void ExecuteFunction(DDModuleAgreement Agreement, DDParam* Param);

	//执行反射方法
	void ExecuteFunction(DDObjectAgreement Agreement, DDParam* Param);

	//注册调用接口
	template<typename RetType, typename... VarTypes>
	DDCallHandle<RetType, VarTypes...> RegisterCallPort(FName CallName);

	//注册方法接口
	template<typename RetType, typename... VarTypes>
	DDFunHandle RegisterFunPort(int32 ModuleID, FName CallName, TFunction<RetType(VarTypes...)> InsFun);

	//开启一个协程, 返回true说明开启成功, 返回false说明已经有同对象名同协程任务名的协程存在
	bool StartCoroutine(FName CoroName, DDCoroTask* CoroTask);

	//停止一个协程, 返回true说明停止协程成功, 返回false说明协程早已经停止
	bool StopCoroutine(FName CoroName);

	//停止该对象的所有协程
	void StopAllCorotine();

	// 延迟运行
	template<typename UserClass>
	bool InvokeDelay(FName InvokeName, float DelayTime, UserClass* UserObj, typename FDDInvokeEvent::TUObjectMethodDelegate<UserClass>::FMethodPtr OnMethod);
	// 延迟循环运行
	template<typename UserClass>
	bool InvokeRepeat(FName InvokeName, float DelayTime, float RepeatTime, UserClass* UserObj, typename FDDInvokeEvent::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod);
	
	// 关闭延迟方法
	bool StopInvoke(FName InVokeName);
	// 关闭所有对象下延迟
	void StopAllInvoke();
protected:

	//保存自身的UObject
	UObject* IBody;

	//保存对应的模组
	UDDModule* IModule;

	//保存驱动器
	ADDDriver* IDriver;

	//对象名字
	FName IObjectName;

	//对象类名字
	FName IClassName;

	//对应模组的序号
	int32 ModuleIndex;


};

template<typename RetType, typename... VarTypes>
DDCallHandle<RetType, VarTypes...>
IDDOO::RegisterCallPort(FName CallName)
{
	return IModule->RegisterCallPort<RetType, VarTypes...>(CallName);
}

template<typename RetType, typename... VarTypes>
DDFunHandle IDDOO::RegisterFunPort(int32 ModuleID, FName CallName, TFunction<RetType(VarTypes...)> InsFun)
{
	if (ModuleIndex == ModuleID)
		return IModule->RegisterFunPort<RetType, VarTypes...>(CallName, InsFun);
	else
		return IDriver->RegisterFunPort<RetType, VarTypes...>(ModuleID, CallName, InsFun);
}

template <typename UserClass>
bool IDDOO::InvokeDelay(FName InvokeName, float DelayTime, UserClass* UserObj,
	typename FDDInvokeEvent::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod)
{
	DDInvokeTask* InvokeTask = new DDInvokeTask(DelayTime, false, 0.f);
	InvokeTask->InvokeEvent.BindUObject(UserObj, InMethod);
	return IModule->StartInvoke(GetObjectName(), InvokeName, InvokeTask);
}

template <typename UserClass>
bool IDDOO::InvokeRepeat(FName InvokeName, float DelayTime, float RepeatTime, UserClass* UserObj,
	typename FDDInvokeEvent::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod)
{
	DDInvokeTask* InvokeTask = new DDInvokeTask(DelayTime, true, RepeatTime);
	InvokeTask->InvokeEvent.BindUObject(UserObj, InMethod);
	return IModule->StartInvoke(GetObjectName(), InvokeName, InvokeTask);
}