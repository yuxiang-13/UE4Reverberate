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



	
	// 反射事件--->销毁对象 (对一群操作 -> 销毁一群小兵)
	UFUNCTION()
	void DestroyObject(EAgreementType Agreement, TArray<FName> TargetNameGroup);
	// 反射事件--->激活对象 (对一群操作 -> 激活一群小兵)
	UFUNCTION()
	void EnableObject(EAgreementType Agreement, TArray<FName> TargetNameGroup);
	// 反射事件--->失活对象 对一群操作 -> 失活一群小兵
	UFUNCTION()
	void DisableObject(EAgreementType Agreement, TArray<FName> TargetNameGroup);
	
	
	// 调用模组的方法
	void ExecuteFunction(DDModuleAgreement Agreement, DDParam* Param);

	// 调用对象的方法
	void ExecuteFunction(DDObjectAgreement Agreement, DDParam* Param);
	
public:
	// 这个模组下的 子 模组
	UPROPERTY()
	TArray<UDDModule*> ChildrenModule;

	// 此模组对应的 模组枚举 下面的序号
	UPROPERTY(VisibleAnywhere, Category = "DataDrivem")
	int32 ModuleIndex;

protected:
	// 执行 单个对象的方法
	void ExecuteSelfObject(DDObjectAgreement Agreement, DDParam* Param);
	// 执行其余对象的方法
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
	// 数据模块 指针
	UDDModel * Model;
	// 消息模块 指针
	UDDMessage * Message;
	// 资源模块 指针
	UDDWealth * Wealth;

	
};
