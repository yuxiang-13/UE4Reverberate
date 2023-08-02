// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DDObject/DDActor.h"
#include "ReflectActor.generated.h"

/**
 * 
 */
UCLASS()
class REFLECTDEMO_API AReflectActor : public ADDActor
{
	GENERATED_BODY()
public:
	virtual void DDEnable() override;

	UFUNCTION()
	void AcceptCall(FString InfiStr);

	UFUNCTION()
	void WealthCall(int32 Counter, FString InfoStr, bool InFlag);
protected:
	DDOBJFUNC_ONE(AcceptCall, FString, InfoStr);
};
