// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FWGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class REFLECTDEMO_API UFWGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	
	// Exec 只能在GameMode,Instance,PlayerController内的函数才行
	UFUNCTION(Exec, Category="FrameWork")
	void GIEcho(FString Info, int32 Count);

	// 重写控制台命令
	virtual bool ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor) override;

	
};
