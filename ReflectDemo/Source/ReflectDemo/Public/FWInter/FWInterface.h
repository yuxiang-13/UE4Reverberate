// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FWInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFWInterface : public UInterface
{
	// GENERATED_BODY() // 1 接口的宏是 GENERATED_BODY   UFunction内不能有任何修饰参数

	GENERATED_UINTERFACE_BODY() // 2 要想用反射UFunction，那就需要改成这个 UINTERFACE
};


/**
 * 
 */
class REFLECTDEMO_API IFWInterface
{
	// GENERATED_BODY() // 1 接口的宏是 GENERATED_BODY  UFunction内不能有任何修饰参数

	GENERATED_IINTERFACE_BODY() // 2 要想用反射UFunction，那就需要改成这个 IINTERFACE
	
public:
	// 接口层 声明方法就行，实现要让 继承接口者重写
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category= "Framework")
	void FWFunOne(const FString& HitResult); // 带BlueprintCallable
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category= "Framework") // BlueprintImplementableEvent 只能在子类蓝图中实现
	void FWFunTwo(const FString& HitResult); // 带BlueprintCallable

	
	UFUNCTION(BlueprintNativeEvent, Category= "Framework")
	void FWFunThree(const FString& HitResult);// 不带 BlueprintCallable
	
	UFUNCTION(BlueprintImplementableEvent, Category= "Framework") // BlueprintImplementableEvent 只能在子类蓝图中实现
	void FWFunFour(const FString& HitResult);// 不带 BlueprintCallable

};
