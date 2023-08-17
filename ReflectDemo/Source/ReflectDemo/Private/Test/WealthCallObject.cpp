// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/WealthCallObject.h"

void UWealthCallObject::DDLoading()
{
	Super::DDLoading();
	IsAllowTickEvent = true;
	
	// LoadClassWealth("ViewActor1", "LoadActorClass");
	// LoadClassWealthKind("ViewActor", "LoadKindClass");
	// BuildSingleClassWealth(EWealthType::Actor, "ViewActor1", "BuildActor", ViewTrans);

	TArray<FTransform> SpawnTransform;
	for (int i = 0; i < 3; ++i)
	{
		SpawnTransform.Push(FTransform(ViewTrans.GetLocation() + FVector(OffsetValue * i, 0.f, 0.f)));
	}
	// BuildKindClassWealth(EWealthType::Actor, "ViewActor", "BuildActorKind", SpawnTransform);

	// BuildMultiClassWealth(EWealthType::Actor, "ViewActor2" ,3, "BuildActorMulti", SpawnTransform);

	StartCoroutine("BuildObjectTest", BuildObjectTest());
}

void UWealthCallObject::DDTick(float DeltaSeconds)
{
	Super::DDTick(DeltaSeconds);

	// if (SingleActor)
	// {
	// 	SingleActor->AddActorWorldRotation(FRotator(1.f, 0.f, 0.f));
	// }
	for (int i = 0; i < KindActors.Num(); i++)
	{
		KindActors[i]->AddActorWorldRotation(FRotator(1.f, 0.f, 0.f));
	}
}


void UWealthCallObject::BuildActorMulti(FName BackName, TArray<AActor*> BackActor)
{
	DDH::Debug(10) << BackName << DDH::Endl();
	KindActors = BackActor;
}

void UWealthCallObject::BuildSingleObject(FName BackName, UObject* BackObject)
{
	DDH::Debug(10) << "BuildSingleObject --> " << BackName << DDH::Endl();

	IDDOO* InstPtr = Cast<IDDOO>(BackObject);
	if (InstPtr)
		InstPtr->RegisterToModule(ModuleIndex);
}

void UWealthCallObject::BuildKindObject(TArray<FName> BackNames, TArray<UObject*> BackObjects)
{
	for (int i = 0; i < BackObjects.Num(); ++i)
	{
		DDH::Debug(10) << "BuildKindObject --> " << BackNames[i] << DDH::Endl();

		IDDOO* InstPtr = Cast<IDDOO>(BackObjects[i]);
		if (InstPtr)
			InstPtr->RegisterToModule(ModuleIndex);
	}
}

void UWealthCallObject::BuildMultiObject(FName BackName, TArray<UObject*> BackObjects)
{
	DDH::Debug(10) << "BuildMultiObject --> " << BackName << DDH::Endl();

	for (int i = 0; i < BackObjects.Num(); ++i)
	{
		IDDOO* InstPtr = Cast<IDDOO>(BackObjects[i]);
		if (InstPtr)
			InstPtr->RegisterToModule(ModuleIndex);
	}
}

DDCoroTask* UWealthCallObject::BuildObjectTest()
{
	DDCORO_PARAM(UWealthCallObject);

#include DDCORO_BEGIN()

	D->BuildSingleClassWealth(EWealthType::Object, "ViewObject2", "BuildSingleObject");

#include DDYIELD_READY()
	DDYIELD_RETURN_SECOND(10.f);

	D->BuildKindClassWealth(EWealthType::Object, "ViewObject", "BuildKindObject");

#include DDYIELD_READY()
	DDYIELD_RETURN_SECOND(10.f);

	D->BuildMultiClassWealth(EWealthType::Object, "ViewObject3", 3, "BuildMultiObject");


#include DDCORO_END()
}


void UWealthCallObject::BuildActorKind(TArray<FName> BackName, TArray<AActor*> BackActor)
{
	for (int i = 0; i < BackName.Num(); i++)
	{
		DDH::Debug(10) << BackName[i] << DDH::Endl();
	}
	KindActors = BackActor;
}

void UWealthCallObject::BuildActor(FName BackName, AActor* BackActor)
{
	DDH::Debug(10) << BackName << DDH::Endl();
	SingleActor = BackActor;
}
void UWealthCallObject::LoadActorClass(FName BackName, UClass* BackWealth)
{
	GetDDWorld()->SpawnActor<AActor>(BackWealth, ViewTrans);
}

void UWealthCallObject::LoadKindClass(TArray<FName> BackNames, TArray<UClass*> BackWealths)
{
	
	for (int i = 0; i < BackWealths.Num(); i++)
	{
		DDH::Debug(10) << BackNames[i] << DDH::Endl();
		
		GetDDWorld()->SpawnActor<AActor>(BackWealths[i], ViewTrans.GetLocation() + FVector(OffsetValue * i, 0.f, 0.f), FQuat::Identity.Rotator());
	}
}
