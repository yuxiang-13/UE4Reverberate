// Fill out your copyright notice in the Description page of Project Settings.


#include "DeleEvent/FWDEActor.h"

// Sets default values
AFWDEActor::AFWDEActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AFWDEActor::RegFunDel(FWDE_DY_Sl_Zero TargetFun)
{
	TargetFun.ExecuteIfBound(); // ExecuteIfBound => 绑定后执行
}

void AFWDEActor::RegFuncOne(TFunction<void(FString)> TargetTFunOne)
{
	// 3步： 赋值 传过来的 函数指针
	TFunOne = TargetTFunOne;
	// 4步： 执行函数指针 (绑定上后，任何地方都能调用了)
	TFunOne(FString("RegFuncOne"));
}


// Called when the game starts or when spawned
void AFWDEActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFWDEActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

