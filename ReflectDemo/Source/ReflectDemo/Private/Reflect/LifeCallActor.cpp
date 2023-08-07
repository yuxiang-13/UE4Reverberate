// Fill out your copyright notice in the Description page of Project Settings.


#include "Reflect/LifeCallActor.h"

void ALifeCallActor::DDInit()
{
	Super::DDInit();

	IsAllowTickEvent = true;

	// DDH::Debug(10) << " DDInit " << DDH::Endl();
}

void ALifeCallActor::DDLoading()
{
	Super::DDLoading();
	// DDH::Debug(10) << " DDLoading " << DDH::Endl();
}

void ALifeCallActor::DDRegister()
{
	Super::DDRegister();
	// DDH::Debug(10) << " DDRegister " << DDH::Endl();

	TFunction<int32(FString)> InFun = [this](FString InfoStr)
	{
		return RegTest(InfoStr);
	};

	
	// 把这个方法  注册到这个模组下消息模块，内的名字叫 "RegCall" 的消息节点
	RegFunHandle = RegisterFunPort<int32, FString>((int32)ERCGameModule::Player, "RegCall", InFun);
}

int32 ALifeCallActor::RegTest(FString InfoStr)
{
	DDH::Debug(0.f) << GetObjectName() << " RegFun- - - -> get Reg Call =" << InfoStr << DDH::Endl();
	return 123;
}


void ALifeCallActor::DDEnable()
{
	Super::DDEnable();
	// DDH::Debug(10) << " DDEnable " << DDH::Endl();

	// TestReflect(ModuleIndex, "TestReflect", 13, "happy", true);

	// TestReflectParam* ResultParam = TestReflectRT((int32)ERCGameModule::HUD, "TestReflect", 13, "happy", true);
	// DDH::Debug(10) << " ResultParam ---> " << ResultParam->BackResult() << DDH::Endl();
	// delete ResultParam;


	// HappyParam((int32)ERCGameModule::HUD, "TestNoParam");


	
}

void ALifeCallActor::DDTick(float DeltaSeconds)
{
	Super::DDTick(DeltaSeconds);
	// DDH::Debug(0) << " DDTick " << DDH::Endl();

	TimeCounter++;
	if (TimeCounter < 3)
	{
		// DDH::Debug(10) << " DDTick --->>>" << TimeCounter << DDH::Endl();
	} else if (TimeCounter == 450) 
	{
		// 如果有设置调用接口与和方法接口，建议在DDUnRegister生命周期函数时候 写注销方法
		RegFunHandle.UnRegister();
		
		// DDDestroy();
		// DDH::Debug(10) << " DDDestroy --->>>" << DDH::Endl();
	}
}



void ALifeCallActor::DDDisable()
{
	Super::DDDisable();
	// DDH::Debug(10) << " DDDisable " << DDH::Endl();
}

void ALifeCallActor::DDUnRegister()
{
	Super::DDUnRegister();
	// DDH::Debug(10) << " DDUnRegister " << DDH::Endl();
}

void ALifeCallActor::DDUnLoading()
{
	Super::DDUnLoading();
	// DDH::Debug(10) << " DDUnLoading " << DDH::Endl();
}

void ALifeCallActor::DDRelease()
{
	Super::DDRelease();
	// DDH::Debug(10) << " DDRelease " << DDH::Endl();
}

void ALifeCallActor::AcceptCall(FString InfiStr)
{
	DDH::Debug(10) << GetObjectName() << " - - - ->" << InfiStr << DDH::Endl();
}