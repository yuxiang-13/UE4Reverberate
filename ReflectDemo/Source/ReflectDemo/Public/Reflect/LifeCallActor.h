// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DDObject/DDActor.h"
#include "LifeCallActor.generated.h"

/**
 * 
 */
UCLASS()
class REFLECTDEMO_API ALifeCallActor : public ADDActor
{
	GENERATED_BODY()
public:
	virtual void DDInit() override;
	virtual void DDLoading() override;
	virtual void DDRegister() override;
	virtual void DDEnable() override;
	virtual void DDTick(float DeltaSeconds) override;

	
	virtual void DDDisable() override;//失活对象1 d
	
	virtual void DDUnRegister() override;//注销数据或者事件
	
	virtual void DDUnLoading() override;//销毁绑定资源
	
	virtual void DDRelease() override;//释放自己

	UFUNCTION()
	void AcceptCall(FString InfiStr);


	// 注册 函数
	int32 RegTest(FString InfoStr);
protected:
	float TimeCounter = 0;

	// 1 方法句柄
	DDFunHandle RegFunHandle;
protected:
	// 使用  宏定义
	DDMODFUNC_THREE(TestReflect, int32, Counter, FString, InfoStr, bool, BackResult);

	DDMODFUNC(HappyParam);
};

