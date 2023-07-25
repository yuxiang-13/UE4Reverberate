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

	// 获取对象名
	FName GetObjectName();
	
	// 获取类名
	FName GetClassName();

	// 获取模组Index
	int32 GetModuleIndex() const;

	// 获取Object主体
	UObject* GetObjectBody() const;

	// 从外部指定模组
	void AssignModule(UDDModule* Mod);
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
