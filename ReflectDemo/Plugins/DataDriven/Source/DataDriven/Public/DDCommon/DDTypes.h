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




#pragma region LifeTimePart
	
// BaseObject 函数方法的 生命周期
UENUM()
enum class EBaseObjectLife: uint8
{
	None = 0, // 初始化状态
	Init,
	Loading,
	Register,
	Enable,
	Disable,
	UnRegister,
	UnLoading
};
// BaseObject 状态的 生命周期
UENUM()
enum class EBaseObjectState: uint8
{
	Active = 0, // 激活进程 的状态
	Stable, // 稳定进程 的状态
	Destroy // 销毁进程 的状态
};

#pragma endregion


// 反射系统代码块
#pragma region ReflectRart


// 对象查找协议
UENUM()
enum class EAgreementType:uint8
{
	SelfObject,		   // 通过传这个objectName,找到对应按个唯一的Object
	OtherObject,       //给传入的对象之外的对象通信
	ClassOtherObject,  //给传入的对象的相同类的 其他对象(不包括自己) 通信,调用这个方法要确保传过来的对象都是同一类的,如果不同类就多次通信
	
	SelfClass,         //给这个类的所有对象通信
	OtherClass,        //给这个类之外的类通信
	All                //给所有的对象通信
};

// 调用结果，项目开发时，请确保每次都能第哦那个用成功
UENUM()
enum class ECallResult: uint8
{
	NoModule = 0, // 缺失模组
	LackObject, // 缺失对象
	NoFunction, // 缺失方法
	Succeed // 调用成功
};

// 通信参数结构体
struct DDParam
{
public:
	// 调用结果
	ECallResult CallResult;
	// 参数指针
	void *ParamPtr;
};

// 通信协议，Module模组方法
struct DDModuleAgreement
{
public:
	// 模组ID
	int32 ModuleIndex;
	// 方法名
	FName FunctionName;
};

// 通信协议，DDOO (对象) 方法
struct DDObjectAgreement
{
public:
	// 模组ID
	int32 ModuleIndex;
	// 协议类型
	EAgreementType AgreementType;
	// 对象组名
	TArray<FName> ObjectGroup;
	// 方法名
	FName FunctionName;
};

#pragma endregion





