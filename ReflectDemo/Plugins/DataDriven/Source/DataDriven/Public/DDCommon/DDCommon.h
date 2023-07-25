// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "DDCommon/DDDefine.h"
#include "DDGame/DGCommon.h"

#include "DDCommon.generated.h"

class ADDDriver;


// 封装打印接口
namespace DDH
{
	//*********
	// DDRecord & --->>  TMD，引用实际可不是指针！！！可以把引用认为是一个变量的别名，就和#define ...，typedef ...很像。  所以只是别名，不是指针，不能return *指针
	//*********
	FORCEINLINE DDRecord& Debug(float InTime = 3000.f, FColor InColor = FColor::Yellow)
	{
		DDRecord::Get()->PatternID = 0;
		DDRecord::Get()->InitParam(InTime, InColor);

		// DDRecord::Get() 返回的是一个共享指针，而函数返回需要是引用，所以需要解引用，加 *
		return *DDRecord::Get();
	}
	FORCEINLINE DDRecord& Debug(FColor InColor = FColor::Yellow)
	{
		return Debug(3000.f, InColor);
	}

	// Log
	FORCEINLINE DDRecord& Log()
	{
		DDRecord::Get()->PatternID = 1;
		return *DDRecord::Get();
	}

	// Warning
	FORCEINLINE DDRecord& Warning()
	{
		DDRecord::Get()->PatternID = 2;
		return *DDRecord::Get();
	}

	// Error
	FORCEINLINE DDRecord& Error()
	{
		DDRecord::Get()->PatternID = 3;
		return *DDRecord::Get();
	}

	FORCEINLINE DDRecord& Endl()
	{
		return *DDRecord::Get();
	}

	// DDH::GetEnumValueAsString<ERCGameModule>("ERCGameModule", ERCGameModule::HUD);
	// 将传入的Enum值对应的Value值，转化成FString值
	template<typename TEnumType>
	FORCEINLINE FString GetEnumValueAsString(const FString& EnumName, TEnumType Value)
	{
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
		if (!EnumPtr)
		{
			return FString("InValue");
		} else
		{
			return EnumPtr->GetNameByValue((int32)Value);
		}
	};

	// 将传入的Enum值转化成对应FName输出
	template<typename TEnumType>
	FORCEINLINE FName GetEnumValueAsName(const FString& EnumName, TEnumType Value)
	{
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
		if (!EnumPtr)
		{
			return FName("InValue");
		} else
		{
			return FName(* EnumPtr->GetNameByValue((int32)Value));
		}
	}

	// 将传入的FName，输出Enum的值Index
	template<typename TEnumType>
	FORCEINLINE TEnumType GetEnumValueFromName(const FString& EnumName, FName Value)
	{
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
		if (!EnumPtr)
		{
			return TEnumType(0);
		} else
		{
			return (TEnumType)EnumPtr->GetIndexByName(Value);
		}
	};

	// 根据传入的FName，输出对应的Enum序号
	FORCEINLINE int32 GetEnumIndexFromName(const FString& EnumName, FName Value)
	{
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
		if (!EnumPtr)
		{
			return -1;
		} else
		{
			return EnumPtr->GetIndexByName(Value);
		}
	}
}



/**
 * 
 */
UCLASS()
class DATADRIVEN_API UDDCommon : public UObject
{
	GENERATED_BODY()
public:
	static UDDCommon* Get();

	// 设置驱动器
	void InitDriver(ADDDriver* InDriver);
	// 获取驱动器
	ADDDriver* GetDriver();
private:
	static UDDCommon* DDInst;
	
	ADDDriver* Driver;
};
