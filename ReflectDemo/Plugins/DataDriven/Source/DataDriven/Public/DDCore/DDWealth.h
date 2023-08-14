// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DDMM.h"
#include "Engine/StreamableManager.h"
#include "UObject/NoExportTypes.h"
#include "DDWealth.generated.h"

// 声明 加载Object节点，cpp中进行实现
struct ObjectSingleLoadNode;

// 声明 加载Object节点，cpp中进行实现
struct ObjectKindLoadNode;

UCLASS()
class DATADRIVEN_API UDDWealth : public UObject, public IDDMM
{
	GENERATED_BODY()
public:
	// 模块生命周期函数
	virtual void WealthInit();
	virtual void WealthBeginPlay();
	virtual void WealthTick(float DeltaSeconds);

	void AssignData(TArray<UWealthData*>& InWealthData);
	
	// 外部方法，单纯获取资源链接
	FWealthURL* GainWealthURL(FName WealthName);
	// 获取一种了类型的资源URL
	void GainWealthURL(FName WealthKind, TArray<FWealthURL*>& OutURL);

	// 加载Object类型资源接口
	void LoadObjectWealth(FName WealthName, FName ObjectName, FName FunName);
	void LoadObjectWealthKind(FName WealthKind, FName ObjectName, FName FunName);
protected:
	// 获取单个结构体
	FObjectWealthEntry* GetObjectSingleEntry(FName WealthName);
	TArray<FObjectWealthEntry*> GetObjectKindEntry(FName WealthKind);
	
	// 处理加载单个Object节点的方法
	void DealObjectSingleLoadStack();
	// 处理批量加载Object节点的方法
	void DealObjectKindLoadStack();
protected:
	// 资源组
	TArray<UWealthData*> WealthData;

	UPROPERTY()
	TArray<UUserWidget*> GCWidgetGroup;

	// 加载器
	FStreamableManager WealthLoader;
	// 加载节点队列
	TArray<ObjectSingleLoadNode*> ObjectSingleLoadStack;
	// 加载节点队列
	TArray<ObjectKindLoadNode*> ObjectKindLoadStack;
protected:
	// 加载Object反射 回调函数
	DDOBJFUNC_TWO(BackObjectWealth, FName, BackName, UObject*, BackWealth);
	// 加载Object一种类型的数组，反射 回调函数
	DDOBJFUNC_TWO(BackObjectWealthKind, TArray<FName>, BackNames, TArray<UObject*>, BackWealths);
};
