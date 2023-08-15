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
	BuildKindClassWealth(EWealthType::Actor, "ViewActor", "BuildActorKind", SpawnTransform);
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
