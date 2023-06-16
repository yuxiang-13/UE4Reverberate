// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FWCommon.generated.h"


// 用于打印的 命名空间
namespace FWHelper
{
	FORCEINLINE void Debug(FString Message, float Duration = 500.f)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::Yellow, Message);
	}
}
/**
 * 
 */
UCLASS()
class REFLECTDEMO_API UFWCommon : public UObject
{
	GENERATED_BODY()
	
};
