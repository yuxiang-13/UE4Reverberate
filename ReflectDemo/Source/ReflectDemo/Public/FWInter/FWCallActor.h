// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FWCallInterface.h"
#include "GameFramework/Actor.h"
#include "FWCallActor.generated.h"

UCLASS()
class REFLECTDEMO_API AFWCallActor : public AActor, public IFWCallInterface // 继承接口类
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFWCallActor();

	// 子类重写 纯虚函数
	virtual void FWCallFun() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
