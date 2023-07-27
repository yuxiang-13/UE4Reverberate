// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DDOO.h"
#include "UObject/NoExportTypes.h"
#include "DDObject.generated.h"

/**
 * 
 */
UCLASS()
class DATADRIVEN_API UDDObject : public UObject, public IDDOO
{
	GENERATED_BODY()
public:	

	// 重写释放函数
	virtual void DDRelease() override;
};
