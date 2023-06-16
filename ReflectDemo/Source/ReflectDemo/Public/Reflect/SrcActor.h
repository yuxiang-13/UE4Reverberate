// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SrcActor.generated.h"

UCLASS()
class REFLECTDEMO_API ASrcActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASrcActor();
public:
	UPROPERTY(EditAnywhere)
	FString ActorName;
	UPROPERTY(EditAnywhere)
	bool IsActive;

	UFUNCTION()
	void UFuncOne();

	// 第二个参数是 引用类型，当作返回值
	UFUNCTION()
	void UFuncTwo(FString InfoStr, int32& Count);

	// 形参都是值传递，返回值bool
	UFUNCTION()
	bool UFuncThree(FString InfoStr, int32 Count);

	
	// 形参含引用传递，返回值int32
	UFUNCTION()
	int32 UFuncFour(FString InfoStr, int32& Count);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
