// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DDObject/DDActor.h"
#include "ReflectActor.generated.h"


// 存储任意类型的 数据结构
struct AnyElement
{
	// 元素父结构体
	struct BaseElement
	{
	public:
		virtual ~BaseElement() {}; // 父类 析构函数必须加 虚构
	};

	// 实际存储值的 结构体
	template<typename T>
	struct ValueElement : public BaseElement
	{
	public:
		T Value;
		ValueElement(const T& InValue) : Value(InValue) {};
	};

public:
	// 父结构体  类型指针，用于指向 子结构体实例化后的  地址
	BaseElement* ElementPtr;
	// 构造
	AnyElement() : ElementPtr(NULL) {};
	/*
	 * 1 <T> 表示 手动指定模板类型
	 * 2 ElementPtr(new ValueElement) 表示 c++泛型，父类指针指向子类
	 */
	template<typename T>
	AnyElement(const T& InValue) : ElementPtr( new ValueElement<T>(InValue) ) {};
	~AnyElement() { delete ElementPtr; };

	//  获取保存的变量  （也可通过 父声明虚函数，子类重写虚函数，就可以通过指向子类的父类型指针，直接触发子类函数）
	//  1 继续模板
	//  2 向下转型，不安全
	template<typename T>
	T & Get()
	{
		ValueElement<T>* SubPtr = static_cast<ValueElement<T>*>(ElementPtr);
		return SubPtr->Value;
	}
	
};


UCLASS()
class REFLECTDEMO_API AReflectActor : public ADDActor
{
	GENERATED_BODY()
public:
	AReflectActor();
	
	// 2 重写注册声明函数
	virtual void DDRegister() override;
	
	virtual void DDEnable() override;

	virtual void DDTick(float DeltaSeconds) override;

	UFUNCTION()
	void AcceptCall(FString InfiStr);

	UFUNCTION()
	void WealthCall(int32 Counter, FString InfoStr, bool InFlag);

	
	UFUNCTION()
	int32 WealthCall111(int32 Counter, FString InfoStr, bool InFlag);
	
	template<typename  RetType, typename ... VarTypes>
	void ReFunList(TFunction<RetType(VarTypes ...)> InsertFun);
	
protected:
	DDOBJFUNC_ONE(AcceptCall, FString, InfoStr);
protected:
	TArray<AnyElement*> ElementList;

	TArray<DDAnyFun*> FunList;

	//  1 声明句柄
	DDCallHandle<int32, FString> RegCallHandle;

	int32 TimeCounter;
};

template <typename RetType, typename ... VarTypes>
void AReflectActor::ReFunList(TFunction<RetType(VarTypes...)> InsertFun)
{
	FunList.Push(new DDAnyFun(InsertFun));
}
