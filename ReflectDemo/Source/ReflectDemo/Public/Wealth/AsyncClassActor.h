// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LatentActions.h"
#include "Engine/StreamableManager.h"
#include "GameFramework/Actor.h"
#include "AsyncClassActor.generated.h"

struct FStreamableManager;
struct FStreamableHandle;

// 定义一个单播委托
DECLARE_DELEGATE_OneParam(FDDLoadCallBack, UClass * )
class FDDLoadAction : public FPendingLatentAction
{
	
public:
	// 资源路径
	FSoftObjectPath SoftObjectPath;
	// 加载流送
	FStreamableManager* StreamableManager;
	TSharedPtr<FStreamableHandle> LoadHandle;

	// 执行的方法名
	FName ExecutionFunction;
	int32 OutputLink;

	// 所绑定的 回调函数 对应的Object
	FWeakObjectPtr CallbackTarget;
	
	// 自己定义的单播委托
	FDDLoadCallBack LoadCallBack;

public:
	// 1 资源路径  2 传过来的回调函数  3 潜在事件
	FDDLoadAction(const FSoftObjectPath& InSoftObjectPath, FDDLoadCallBack InLoadCallBack, const FLatentActionInfo& InLatentInfo)
		: SoftObjectPath(InSoftObjectPath)
		, LoadCallBack(InLoadCallBack)
		, ExecutionFunction(InLatentInfo.ExecutionFunction)
		, OutputLink(InLatentInfo.Linkage)
		, CallbackTarget(InLatentInfo.CallbackTarget)
	{
		
		// 创建资源加载管理器
		StreamableManager = new FStreamableManager();
		// 执行异步加载，添加资源链接数组 和 加载完成回调
		LoadHandle = StreamableManager->RequestAsyncLoad(SoftObjectPath);
		
	};

	virtual ~FDDLoadAction()
	{
		if (LoadHandle.IsValid())
		{
			LoadHandle->ReleaseHandle();
		}
	}
};


UCLASS()
class REFLECTDEMO_API AAsyncClassActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAsyncClassActor();

	virtual void Tick(float DeltaTime) override;
public:
    UPROPERTY(EditAnywhere)
	TArray<TSoftClassPtr<UObject>> ClassWealthPaths;

protected:
	virtual void BeginPlay() override;

	void LoadWealthComplated();
protected:
	// 加载句柄
	TSharedPtr<FStreamableHandle> WealthHandle;

	// 加载流
	FStreamableManager* WealthLoader;

	// 保存加载好后，实例化后的 蓝图
	TArray<AActor*> WealthActors;


	// 计时器
	float TimeCounter;
	// 计数器
	int32 IndexCounter;
	
};
