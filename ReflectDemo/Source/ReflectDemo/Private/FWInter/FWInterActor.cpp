// Fill out your copyright notice in the Description page of Project Settings.


#include "FWInter/FWInterActor.h"

#include "Common/FWCommon.h"

// Sets default values
AFWInterActor::AFWInterActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AFWInterActor::FWFunOne_Implementation(const FString& HitResult)
{
	IFWInterface::FWFunOne_Implementation(HitResult);
	FWHelper::Debug(FString("- - - >>>    FWFunOne_Implementation"), 10.f);
}

void AFWInterActor::FWFunThree_Implementation(const FString& HitResult)
{
	IFWInterface::FWFunThree_Implementation(HitResult);
	FWHelper::Debug(FString("- - - >>>    FWFunThree_Implementation"), 10.f);
}

// Called when the game starts or when spawned
void AFWInterActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFWInterActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

