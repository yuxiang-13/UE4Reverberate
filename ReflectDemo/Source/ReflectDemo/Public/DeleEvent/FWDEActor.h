// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FWDEActor.generated.h"

// 单播多播
DECLARE_DELEGATE(FWDE_Single_Zero)
DECLARE_DELEGATE_TwoParams(FWDE_Single_Two, int32 , FString)
DECLARE_MULTICAST_DELEGATE(FWDE_Multi_Zero)

// 动态单播多播
//  与单播多播区别
//  1 “ 动态代理与标准代理之间区别:动态代理支持序列化,因此可以在蓝图中使用,而标准的却不能
//  2 动态委托声明时候，需要加 ”;“
DECLARE_DYNAMIC_DELEGATE(FWDE_DY_Sl_Zero);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWDE_Dy_Sl_One, FString, InfoStr); // FString => 参数类型  InfoStr => 参数名

UCLASS()
class REFLECTDEMO_API AFWDEActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFWDEActor();

	// 让委托变量作为参数
	// 动态代理支持序列化,因此可以在蓝图中使用,所以，加了UFUNCTION  就只能传递 动态委托
	UFUNCTION(BlueprintCallable, Category= "FrameWork")
	void RegFunDel(FWDE_DY_Sl_Zero TargetFun);

	// TFunction传递函数
	// 2步： 函数形参直接 就是函数指针
	void RegFuncOne(TFunction<void(FString)> TargetTFunOne);


	// TMemFuncPtrType 传递函数
	//  
	/*  1 TMemFunPtrType<false, UserClass, void(int,FString)> 含义
	 *  作用 => 定义 函数指针
	 *  参数1 bool值是该函数指针  是否为const函数
	 *  参数2 UserClass 绑定哪个对象的
	 *  参数3 void(int,FString) 要传递的函数方法的类型（返回值  形参）
	 *  ::Type  Type 就是这个函数指针，它的参数类型是(int,FString)，返回类型是void
	 *  2 利用typename关键字，对函数指针重新命名为InMethod。
	 */
	template<class UserClass>
	void RegFuncTwo(UserClass* TarObj, typename TMemFunPtrType<false, UserClass, void(int,FString)>::Type InMethod);


	// 方法3： FMethodPtr 的函数传递  method 方法 英 /ˈmeθəd/
	/*
	 * FWDE_Single_Two::TUObjectMethodDelegate<UserClass>::FMethodPtr
	 * 1 FWDE_Single_Two: 单播委托
	 * 2 委托中的  TUObjectMethodDelegate  成员函数的委托指针类型
	 * 3 `<UserClass>` 模板参数是指向委托函数所属对象的指针类型
	 * 4  `FMethodPtr` 是这个结构体中的一个成员变量，保存了指向委托函数的指针。
	 *    这个委托函数必须是以“UserClass”为参数的成员函数。
	 * typename xxx InMethod  实际就是把 xxx 变成一个类型，而不是一个对象
	 */
	template <class UserClass>
	void RegFunThree(UserClass* TarObj, typename FWDE_Single_Two::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod);


	
	// 泛型定义统一接口 FMethodPtr
	// class DelegateType 之后传的是一个泛型(就是模板Template)
	// class userClass 传类指针
	// typename... Vartypes 表示多个类型
	/*
		1 DelegateType::template TUObjectMethodDelegate<UserClass>::FMethodPtr
		1）在C++中，当一个类是模板类时，如果在其内部使用了另一个模板，
			编译器需要知道 template 关键字用于指示后面的 
			TUObjectMethodDelegate<UserClass>::FMethodPtr 是一个模板，而不是成员变量或成员函数。
		2）表示在 DelegateType 类模板内部使用了 TUObjectMethodDelegate<UserClass> 模板，并且 FMethodPtr 是指向该模板的委托函数的指针。
		3）使用 typename 关键字在 typename DelegateType::template TUObjectMethodDelegate<UserClass>::FMethodPtr 前面是为了告诉编译器
			FMethodPtr 是一个类型，而不是一个成员函数或静态变量。
		4）总结起来，DelegateType::template 的作用是指示编译器 DelegateType 是一个模板类，并在该模板类内部使用了另一个模
	 */
	template<class DelegateType, class UserClass, typename... Vartypes>
	void RegFunFour(UserClass* TarObj, typename DelegateType::template TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, Vartypes... Vars);

	
	
	// 泛型定义统一接口  
	// RetType =>  返回值
	// VarTypes => 不定参
	template<typename RetType, typename... VarTypes>
	void RegFunFive_HasB_Params(TFunction<RetType(VarTypes ...)> TarFuncTion, VarTypes... Vars)
	{
		TarFuncTion(Vars...);
	}

	
	template<typename RetType, typename... VarTypes>
	void RegFunFive_ASelf_Params(TFunction<RetType(VarTypes ...)> TarFuncTion)
	{
		bool returnResult = TarFuncTion(FString("A Self Params"), 300);
		
		if (returnResult)
		{
			FWHelper::Debug(FString("- - - >>> RegFunFive_ASelf_Params 函数 返回真 真 真 "), 10.f);
		} else
		{
			FWHelper::Debug(FString("- - - >>> RegFunFive_ASelf_Params 函数 返回假 假 假 "), 10.f);
		}
	}
public:
	// 委托变量   BlueprintAssignable Assignable 可分配的;英/əˈsaɪnəbl/
	// 暴露该属性来在蓝图中进行赋值,  仅能用于 【Multicast代理】
	UPROPERTY(BlueprintAssignable)
	FWDE_Dy_Sl_One FDSZFunc;

private:
	/* 1步： 定义函数指针
	 * < > 泛型
	 * void => 表示 函数返回值是 void
	 * (FString) => 表示 函数行参 只有一个 FString类型
	 */
	// 这样就定义好了一个函数指针的  变量
	TFunction<void(FString)> TFunOne;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

// 把传过来的 函数指针，绑定到委托上
template <class UserClass>
void AFWDEActor::RegFuncTwo(UserClass* TarObj,
	typename TMemFunPtrType<false, UserClass, void(int, FString)>::Type InMethod)
{
	// // 声明单播委托
	// FWDE_Single_Zero ExeDel;
	// // 把函数指针绑定到 单播委托
	// ExeDel.BindUObject(TarObj, InMethod, 54, FString("hahaha"));
	// // 触发
	// ExeDel.ExecuteIfBound();

	
	// 声明委托变量
	FWDE_Single_Two ExeDel;
	ExeDel.BindUObject(TarObj, InMethod);
	// 触发
	ExeDel.ExecuteIfBound(36, FString("- - ->>  I ---"));
}

template <class UserClass>
void AFWDEActor::RegFunThree(UserClass* TarObj,
	typename FWDE_Single_Two::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod)
{
	// // 声明委托变量
	// FWDE_Single_Two ExeDel;
	// ExeDel.BindUObject(TarObj, InMethod);
	// // 触发
	// ExeDel.ExecuteIfBound(36, FString("- - ->>  I ---"));

	
	// 声明单播委托
	FWDE_Single_Zero ExeDel;
	// 把函数指针绑定到 单播委托
	ExeDel.BindUObject(TarObj, InMethod, 54, FString("hahaha"));
	// 触发
	ExeDel.ExecuteIfBound();
}


template <class DelegateType, class UserClass, typename ... Vartypes>
void AFWDEActor::RegFunFour(UserClass* TarObj,
 // typename DelegateType::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, Vartypes... Vars)
	typename DelegateType::template TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, Vartypes... Vars)
{
	// 声明单播委托
	FWDE_Single_Zero ExeDel;
	// 把函数指针绑定到 单播委托
	ExeDel.BindUObject(TarObj, InMethod, Vars... ); // 传入可变参
	// 触发
	ExeDel.ExecuteIfBound();
}

