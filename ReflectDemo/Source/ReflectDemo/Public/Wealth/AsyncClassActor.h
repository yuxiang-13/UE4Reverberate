// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AsyncClassActor.generated.h"

struct FStreamableManager;
struct FStreamableHandle;

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
