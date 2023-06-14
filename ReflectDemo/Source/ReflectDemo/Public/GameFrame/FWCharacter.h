// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FWCharacter.generated.h"

UCLASS()
class REFLECTDEMO_API AFWCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFWCharacter();

	// BlueprintCallable c++声明 ，蓝图调用
	// 第二个参数是引用  （1 蓝图函数参数居然能返回多个参数）
	// 函数形参使用引用，1 加const = 正常函数参数，2不加const=函数体返回值
	UFUNCTION(BlueprintCallable, Category="FrameWork")
	void CAFuncOne(int32 Input, bool & Output);

	// 蓝图实现，c++调蓝图
	UFUNCTION(BlueprintImplementableEvent, Category="FrameWork")
	void CAFuncTwo(int32 Input, bool & Output);

	// C++蓝图 均可实现，c++要加ImplementEvent
	UFUNCTION(BlueprintNativeEvent, Category="FrameWork")
	void CAFuncThree(int32 Input, bool & Output);

	// Exec 只能在GameMode,Instance,PlayerController内的函数才行
	UFUNCTION(Exec, Category="FrameWork")
	void CAFuncFour(FString Info, int32 Count);

#if WITH_EDITOR //editor
	// 实现 ： 编辑器模式下，成员属性改变时，出发这个函数
	// 为了实现，修改场景内变量时，可以直接生成一定数量的墙
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FrameWork")
	AActor* CAOne;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FrameWork")
	TSubclassOf<AActor> CATwo;

	// 对象软引用，就是一个路径，需要tryload
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FrameWork")
	TSoftObjectPtr<AActor> CAThree;
	// 对象类的类型软引用，就是一个路径，需要TryLoadClass
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FrameWork")
	TSoftClassPtr<AActor> CAFour;
	// 筛选 MetaClass = "Actor" 一定是存在的类，不然崩溃
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MetaClass = "Actor"))
	FSoftClassPath CAFourOne111;

	// 资源的引用.对TSoftObjectPtr的封装.包含对象路径以及弱指针
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FrameWork")
	TAssetPtr<class UTexture2D> CAFive;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
