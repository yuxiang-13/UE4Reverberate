// Fill out your copyright notice in the Description page of Project Settings.


#include "FWInter/FWCallActor.h"

#include "Common/FWCommon.h"

// Sets default values
AFWCallActor::AFWCallActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AFWCallActor::FWCallFun()
{
	FWHelper::Debug(FString("- - - >>>    FWCallFun"), 10.f);
}

// Called when the game starts or when spawned
void AFWCallActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFWCallActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

