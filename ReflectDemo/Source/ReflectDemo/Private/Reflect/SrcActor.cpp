// Fill out your copyright notice in the Description page of Project Settings.


#include "Reflect/SrcActor.h"

#include "Common/FWCommon.h"

// Sets default values
ASrcActor::ASrcActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	ActorName = FString("AsrcActor");
	IsActive = true;
}

void ASrcActor::UFuncOne()
{
	FWHelper::Debug(FString("ASrcActor --->>> UFuncOne  "), 10.f);
}

void ASrcActor::UFuncTwo(FString InfoStr, int32& Count)
{
	FWHelper::Debug(InfoStr + FString("ASrcActor --->>> UFuncTwo  ") + FString::FromInt(Count), 10.f);
	Count = 100;
}

bool ASrcActor::UFuncThree(FString InfoStr, int32 Count)
{
	FWHelper::Debug(InfoStr + FString("ASrcActor --->>> UFuncThree  ") + FString::FromInt(Count), 10.f);
	return true;
}

int32 ASrcActor::UFuncFour(FString InfoStr, int32& Count)
{
	FWHelper::Debug(InfoStr + FString("ASrcActor --->>> UFuncFour  ") + FString::FromInt(Count), 10.f);
	Count = 1000;

	return 2000;
}

// Called when the game starts or when spawned
void ASrcActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASrcActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

