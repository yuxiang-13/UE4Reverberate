// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DDCore/DDModule.h"
#include "UObject/Interface.h"
#include "DDOO.generated.h"

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

public:
	// 对象 注册到框架
	void RegisterToModule(FName ModName, FName ObjName = FName(), FName ClsName = FName());
	// 同上，只传入模组Index
	void RegisterToModule(int32 ModIndex, FName ObjName = FName(), FName ClsName = FName());
	

	// 获取对象名
	FName GetObjectName();
	
	// 获取类名
	FName GetClassName();

	// 获取模组Index
	int32 GetModuleIndex() const;

	// 获取Object主体
	UObject* GetObjectBody() const;

	// 框架获取世界函数(因为某一些类中 通过原始获取世界的方法，会获取不到  -->> Actor类是 放到世界的，所以可以获取世界，继承自Uobject的类，不会放到世界，就获取不到世界)
	UWorld* GetDDWorld() const;

	// 从外部指定模组
	void AssignModule(UDDModule* Mod);

	
	// 激活生命周期函数，激活成功返回true
	bool ActiveLife();
	// 停止生命周期函数，销毁成功返回true，从数据模块注销
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

	// 激活对象
	virtual void OnEnable();
	// 失活对象方法
	virtual void OnDisable();
	// 销毁自己
	void DDDestroy();
public:
	//是否允许帧运行,如果要允许帧运行需要在构造函数或者BeginPlay设置,在UE4里默认为false
	bool IsAllowTickEvent;
	
	//生命周期状态
	EBaseObjectLife LifeState;

	//生命运行状态
	EBaseObjectState RunState;
	
protected:
	// 执行反射方法
	void ExecuteFunction(DDModuleAgreement Agreement, DDParam* Param);
	
	// 执行反射方法
	void ExecuteFunction(DDObjectAgreement Agreement, DDParam* Param);
protected:
	// 保存自身UObject
	UObject* IBody;

	// 保存模组
	UDDModule* IModule;

	// 保存驱动器
	ADDDriver* IDriver;

	// 对象名字
	FName IObjectName;

	// 对象类名字
	FName IClassName;

	// 对应模组的序号
	int32 ModuleIndex;
};
