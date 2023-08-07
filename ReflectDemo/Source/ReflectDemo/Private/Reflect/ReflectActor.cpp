// Fill out your copyright notice in the Description page of Project Settings.


#include "Reflect/ReflectActor.h"

AReflectActor::AReflectActor()
{
	IsAllowTickEvent = true;
}

void AReflectActor::DDRegister()
{
	Super::DDRegister();

	RegCallHandle = RegisterCallPort<int32, FString>("RegCall");
}

void AReflectActor::DDEnable()
{
	Super::DDEnable();

	// AcceptCallParam* Param = AcceptCallRT((int32)ERCGameModule::Center, EAgreementType::SelfObject , "LifeCallActor", "AcceptCall", "Happy EveryDay");
	//
	// DDH::Debug(10) << DDH::GetCallResult(Param->CallResult) << DDH::Endl();
	// delete Param;

	ElementList.Push(new AnyElement(22222333));
	ElementList.Push(new AnyElement( FString("Happy")));
	ElementList.Push(new AnyElement(false));
	ElementList.Push(new AnyElement(FVector(1.f)));

	TFunction<void(FString)> Fun1 = [this](FString InfoStr)
	{
		// 这个函数 不带返回值，所以 不加 return
		AcceptCall(InfoStr);
	};
	ReFunList<void, FString>(Fun1);

	TFunction<int32(int32, FString, bool)> Fun2 = [this](int32 InCounter, FString InInfoStr, bool InInFlag)
	{
		// 这个函数带返回值，所以加 return
		return WealthCall111(InCounter, InInfoStr, InInFlag);
	};
	ReFunList<int32, int32, FString, bool>(Fun2);

	// FunList[0]->Execute<void, FString>(FString(" Happy New Year  "));
	// DDH::Debug(10) << FunList[1]->Execute<int32, int32, FString, bool>(233, FString(" No Way  "), false) << DDH::Endl();
}




void AReflectActor::DDTick(float DeltaSeconds)
{
	Super::DDTick(DeltaSeconds);

	// 接口调用，传参 TimeCounter
	DDH::Debug(0.f) << GetObjectName() << " - - -AcceptCall ->" << RegCallHandle.Execute(FString::FromInt(TimeCounter++)) << DDH::Endl();

	// 测试注销
	// if (TimeCounter == 450)
	// {
	// 	// RegCallHandle.UnRegister();
	// }
}

void AReflectActor::AcceptCall(FString InfiStr)
{
	DDH::Debug(10) << GetObjectName() << " - - -AcceptCall ->" << InfiStr << DDH::Endl();
}

void AReflectActor::WealthCall(int32 Counter, FString InfoStr, bool InFlag)
{
	DDH::Debug(10) << GetObjectName() << " - - -WealthCall ->" << DDH::Endl();
}

int32 AReflectActor::WealthCall111(int32 Counter, FString InfoStr, bool InFlag)
{
	DDH::Debug(10) << " WealthCall111 - - - ->" << DDH::Endl();
	return 7823;
}
