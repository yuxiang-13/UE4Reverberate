// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DecActor.generated.h"

UCLASS()
class REFLECTDEMO_API ADecActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ADecActor();

	// 反射实例化对象
	void ReflectInstance();

	void ControlUFunc();

	// 调用UFunction方法二： TBaseDelegate
	void RunUFuncTwo(); 
	
	// 调用UFunction方法三： UFunction函数指针
	void RunUFuncThree();
protected: 
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	class ASrcActor* SrcAct;
	UObject* SrcObj;
};
