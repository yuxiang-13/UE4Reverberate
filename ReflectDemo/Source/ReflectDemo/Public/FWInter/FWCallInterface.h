// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FWCallInterface.generated.h"

// 带  meta = (CannotImplementInterfaceInBlueprint) 只能够接受UFUNCTION(BlueprintCallable)的方法

// 第二种 不带 meta = (CannotImplementInterfaceInBlueprint)
/*不能够接受UFUNCTION(BlueprintCallable)的方法
* 可以接受   
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	UFUNCTION(BlueprintNativeEvent) 
	UFUNCTION(BlueprintImplementableEvent)
等方法,不需要写成虚函数
子类重写需要在.h和.cpp文件都加上_Implementation后缀
 * 
 */
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UFWCallInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 * 
 */
class REFLECTDEMO_API IFWCallInterface
{
	GENERATED_IINTERFACE_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, Category="Framework")
	virtual void FWCallFun() = 0; // 纯函数，只声明，子类必实现
	
	
};
