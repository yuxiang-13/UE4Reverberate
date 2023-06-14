// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FWInterface.h"
#include "GameFramework/Actor.h"
#include "FWInterActor.generated.h"

UCLASS()
class REFLECTDEMO_API AFWInterActor : public AActor, public  IFWInterface // 继承接口
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFWInterActor();

	// 实现接口。BlueprintNativeEvent 的实现要尾部加 _Implementation
	virtual void FWFunOne_Implementation(const FString& HitResult);
	
	virtual void FWFunThree_Implementation(const FString& HitResult);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
