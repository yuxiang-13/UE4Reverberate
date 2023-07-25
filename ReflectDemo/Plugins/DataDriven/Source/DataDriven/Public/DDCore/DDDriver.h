// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// 引用中央模组
#include "DDCenterModule.h"
#include "DDCommon/DDCommon.h"

#include "DDDriver.generated.h"


UCLASS()
class DATADRIVEN_API ADDDriver : public AActor
{
	GENERATED_BODY()
	
public:	
	ADDDriver();

	virtual void PostInitializeComponents() override;

	virtual void Tick(float DeltaTime) override;

	
	// 驱动器驱动，UPROPERTY的变量发生变化时自动触发函数,函数内让中心模组触发 本模组以及子模组的类型设置
	#if WITH_EDITOR
	
	// 属性修改方法
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

    #endif
	
public:
	UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category="DataDriven")
	USceneComponent* RootScene;

	// 中央模组
	UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category="DataDriven")
	UDDCenterModule* Center;

	// 模组对应的 枚举名字
	UPROPERTY(EditDefaultsOnly, Category="DataDriven")
	FName ModuleType;
	
protected:
	virtual void BeginPlay() override;

	// 判断是不是Tick第一帧
	bool IsBeginePlay;
	
};
