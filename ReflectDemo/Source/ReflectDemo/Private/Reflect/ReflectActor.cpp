// Fill out your copyright notice in the Description page of Project Settings.


#include "Reflect/ReflectActor.h"

void AReflectActor::DDEnable()
{
	Super::DDEnable();

	// AcceptCallParam* Param = AcceptCallRT((int32)ERCGameModule::Center, EAgreementType::SelfObject , "LifeCallActor", "AcceptCall", "Happy EveryDay");
	//
	// DDH::Debug(10) << DDH::GetCallResult(Param->CallResult) << DDH::Endl();
	// delete Param;
}

void AReflectActor::AcceptCall(FString InfiStr)
{
	// DDH::Debug(10) << GetObjectName() << " - - - ->" << InfiStr << DDH::Endl();
}

void AReflectActor::WealthCall(int32 Counter, FString InfoStr, bool InFlag)
{
}
