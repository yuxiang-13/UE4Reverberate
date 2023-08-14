// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DDObject/DDActor.h"
#include "TestWealthActor.generated.h"

/**
 * 
 */
UCLASS()
class REFLECTDEMO_API ATestWealthActor : public ADDActor
{
	GENERATED_BODY()
public:
	virtual void DDEnable() override;
protected:
	DDOBJFUNC(CallWealth)
};
