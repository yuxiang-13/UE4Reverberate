// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DDMM.h"
#include "UObject/NoExportTypes.h"
#include "DDWealth.generated.h"

/**
 * 
 */
UCLASS()
class DATADRIVEN_API UDDWealth : public UObject, public IDDMM
{
	GENERATED_BODY()
public:
	// 模块生命周期函数
	virtual void WealthInit();
	virtual void WealthBeginPlay();
	virtual void WealthTick(float DeltaSeconds);
};