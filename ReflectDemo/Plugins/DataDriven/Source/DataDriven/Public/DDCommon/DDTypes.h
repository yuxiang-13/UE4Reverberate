// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/GameEngine.h"


#include "DDTypes.generated.h"

/**
 * 
 */
UCLASS()
class DATADRIVEN_API UDDTypes : public UObject
{
	GENERATED_BODY()
	
};

// 1 #pragma once
// 大家应该都知道：指定该文件在编译源代码文件时仅由编译器包含（打开）一次。

// 2 #pragma region
// 用来展开、收缩代码区域。在一些函数比较多的文件中适用。

// 框架自定义Debug输出类
#pragma region LogDebug

// 用于绑定QT的Debug类
class DATADRIVEN_API DDRecord
{
private:
	// 自身单例
	static TSharedPtr<DDRecord> RecordInst;

	// 生成的字符串
	FString RecordInfo;
	// 显示时间
	float ShowTime;
	// 显示颜色
	FColor ShowColor;
public:
	// 打印模式： 0【Debug】 1【Log】 2【Warning】 3【Error】
	uint8 PatternID;
	
public:
	// 构造  析构
	inline  DDRecord() {}

	~DDRecord() {}

	static TSharedPtr<DDRecord> Get();

	// 设定参数
	inline void InitParam(float InTime, FColor InColor)
	{
		ShowTime = InTime;
		ShowColor = InColor;
	};
	// 执行打印
	inline void Output()
	{
		switch (PatternID)
		{
		case 0: // 打印到屏幕
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, ShowTime, ShowColor, RecordInfo);
				}
			}
			break;
		
		case 1: // 打印到Log
			{
				UE_LOG(LogTemp, Log, TEXT("%s"), *RecordInfo);
			}
			break;
		case 2:
			{
				UE_LOG(LogTemp, Warning, TEXT("%s"), *RecordInfo);
			}
			break;
		case 3:
			{
				UE_LOG(LogTemp, Error, TEXT("%s"), *RecordInfo);
			}
			break;
		}

		// 清理字符串
		RecordInfo.Empty();
	}

	// 1 重写操作符 <<  【 满足 = DDRecord::Get()类 出发自己类中 << 操作符】
	// 2 *** 【*this对指针解引用，因此使用*this返回的就是一个对象本身】
	// 3 << 函数参数重载，下面是FString参数函数
	inline DDRecord & operator<<(FString Info) { RecordInfo.Append(Info); return *this; }
	inline DDRecord & operator<<(FName Info) { RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord & operator<<(FText Info) { RecordInfo.Append(Info.ToString()); return *this; }
	
	inline DDRecord & operator<<(bool Info) { RecordInfo.Append( Info ? FString("True") : FString("False") ); return *this; }
	
	inline DDRecord & operator<<(const char* Info) { RecordInfo += Info; return *this; }
	inline DDRecord & operator<<(const char Info) { RecordInfo.AppendChar(Info); return *this; }

	inline DDRecord & operator<<(int32 Info) { RecordInfo.Append(FString::FromInt(Info)); return *this; }
	inline DDRecord & operator<<(float Info) { RecordInfo.Append(FString::SanitizeFloat(Info)); return *this; }
	inline DDRecord & operator<<(double Info) { RecordInfo.Append(FString::SanitizeFloat(Info)); return *this; }

	
	inline DDRecord & operator<<(FVector2D Info) { RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord & operator<<(FVector Info) { RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord & operator<<(FRotator Info) { RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord & operator<<(FQuat Info) { RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord & operator<<(FTransform Info) { RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord & operator<<(FMatrix Info) { RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord & operator<<(FColor Info) { RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord & operator<<(FLinearColor Info) { RecordInfo.Append(Info.ToString()); return *this; }

	// 输出字符串，匹配函数参数类型为自身  {使用例子  DDRecord::Get() << FString("ssss) << int << DDRecord::Get()}
	inline void operator<<(DDRecord& Record) { Record.Output(); };
};

#pragma endregion 