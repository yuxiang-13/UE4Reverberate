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
	UDDModule();

	// 创建模块方法
	void CreateManager();
	// 模组 生命周期方法
	virtual void ModuleInit();
	virtual void ModuleBeginPlay();
	virtual void ModuleTick(float DeltaSeconds);

	// 设定模组对应的枚举类型
	void ChangeModuleType(FName ModuleType);
	
	// 注册对象 到  数据模块
	void RegisterObject(IDDOO* ObjectInst);

	// 模组所属对象的 销毁自身
	void ChildDestroy(FName ObjectName);

	// 调用模组的方法
	void ExecuteFunction(DDModuleAgreement Agreement, DDParam* Param);

	// 临时方法，测试反射事件系统
	UFUNCTION()
	void TestReflect(int32 Counter, FString InfoStr, bool& BackResult);
	
	// 临时方法，测试反射事件系统
	UFUNCTION()
	void TestNoParam();
	
public:
	// 这个模组下的 子 模组
	UPROPERTY()
	TArray<UDDModule*> ChildrenModule;

	// 此模组对应的 模组枚举 下面的序号
	UPROPERTY(VisibleAnywhere, Category = "DataDrivem")
	int32 ModuleIndex;

	
protected:
	// 数据模块 指针
	UDDModel * Model;
	// 消息模块 指针
	UDDMessage * Message;
	// 资源模块 指针
	UDDWealth * Wealth;

	
};
