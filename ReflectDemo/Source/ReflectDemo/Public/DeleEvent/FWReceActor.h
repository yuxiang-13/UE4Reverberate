// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FWReceActor.generated.h"

UCLASS()
class REFLECTDEMO_API AFWReceActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFWReceActor();

	// 声明一个 public 函数
	void EchoInfoOne(FString InfoStr);

	// b 中 声明一个函数,一会让 A 绑定这个函数指针
	void EchoInfoTwo(int32 Count, FString InfoStr);

	
	bool EchoInfoThree(FString InfoStr, int32 Count);

protected:

	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
