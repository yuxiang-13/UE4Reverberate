// Fill out your copyright notice in the Description page of Project Settings.


#include "FWInter/FWCallActor.h"

#include "Common/FWCommon.h"
#include "FWInter/FWInterActor.h"
#include "Kismet/GameplayStatics.h"

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

	// 获取另一个Interface的Actor
	TArray<AActor*> ActArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFWInterActor::StaticClass(), ActArray);
	if (ActArray.Num() > 0)
	{
		AFWInterActor* FWInterActor = Cast<AFWInterActor>(ActArray[0]);
		// UE 推荐 调用接口时，直接使用 Execute_ 的
		// FWInterActor->Execute_FWFunThree(FWInterActor, FString(" -- - >>>>  Execute_FWFunThree "));
	}
}

// Called every frame
void AFWCallActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

