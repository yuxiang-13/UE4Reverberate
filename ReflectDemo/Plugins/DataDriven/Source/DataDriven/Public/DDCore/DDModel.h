// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DDMM.h"
#include "UObject/NoExportTypes.h"
#include "DDModel.generated.h"

/**
 * 
 */
UCLASS()
class DATADRIVEN_API UDDModel : public UObject, public IDDMM
{
	GENERATED_BODY()
public:
	// 模块生命周期函数
	virtual void ModelInit();
	virtual void ModelBeginPlay();
	virtual void ModelTick(float DeltaSeconds);
};
