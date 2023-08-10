// Fill out your copyright notice in the Description page of Project Settings.


#include "Reflect/DecActor.h"

#include "Common/FWCommon.h"
#include "Kismet/GameplayStatics.h"
#include "Reflect/RefObj.h"
#include "Reflect/SrcActor.h"
#include "UObject/UnrealTypePrivate.h"

// Sets default values
ADecActor::ADecActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ADecActor::BeginPlay()
{
	Super::BeginPlay();

	// ReflectInstance();
	TArray<AActor*> ActArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASrcActor::StaticClass(), ActArray);
	if (ActArray.Num() > 0) {
		SrcAct = Cast<ASrcActor>(ActArray[0]);
		// 任何都可以 强转基类 UObject
		SrcObj = (UObject *)SrcAct;

		// 反射操作UFunction对象
		// 方法1 FScriptDelegate  无法绑定带返回值的函数
		// ControlUFunc();
		//
		// RunUFuncTwo();

		RunUFuncThree();
	}
}

void ADecActor::ControlUFunc()
{
	FScriptDelegate FunDelegate;
	// 绑定代理 -> UObject 内 Ufunction名字
	FunDelegate.BindUFunction(SrcObj, FName("UFuncOne"));
	// 执行   Process处理 泼赛斯
	// FunDelegate.ProcessDelegate<UObject>(NULL);

	
	FScriptDelegate FunDelegateTwo;
	FunDelegateTwo.BindUFunction(SrcObj, FName("UFuncTwo"));
	// 匿名结构体 (只能使用一次，因为匿名，除了定义，没名字去获取它)
	struct
	{
		FString InfoStr;
		int32 Count;
	} FuncTwoParams;
	FuncTwoParams.InfoStr = FString("ASrcActor====>  UFuncTwo");
	FuncTwoParams.Count = 500;
	// 把结构体地址传过去，就会通过地址偏移进行函数赋值
	FunDelegateTwo.ProcessDelegate<UObject>(&FuncTwoParams);
	// FWHelper::Debug(FString("-  匿名结构体 -> Count 通过函数赋值 从 500 修改成了 ") + FString::FromInt(FuncTwoParams.Count), 10.f);

}

void ADecActor::RunUFuncTwo()
{
	/*
	 * <>内是 所绑定函数的 返回值 + 函数参数
	TBaseDelegate<bool, FString, int32>

	   CreateUFunction(SrcObj, "UFuncThree")
	绑定的Uobject和 UFuncThree
	 */
	TBaseDelegate<bool, FString, int32> FunDelegateThree = TBaseDelegate<bool, FString, int32>::CreateUFunction(SrcObj, "UFuncThree");
	bool DelResult = FunDelegateThree.Execute(FString("ASrcActor ----->  UFunctionThree "), 789);
	if (DelResult)
	{
		//FWHelper::Debug(FString("-  Return True "), 10.f);
	}

}

void ADecActor::RunUFuncThree()
{
	UFunction* FunctionFour = SrcObj->FindFunction(FName("UFuncFour"));
	if (FunctionFour)
	{
		// 匿名结构体 (只能使用一次，因为匿名，除了定义，没名字去获取它)
		struct
		{
			FString InfoStr;
			int32 Count;
		} FuncTwoParams;
		FuncTwoParams.InfoStr = FString("ASrcActor====>  UFuncFour");
		FuncTwoParams.Count = 800;
		// 传递地址
		SrcObj->ProcessEvent(FunctionFour, &FuncTwoParams);

		// 1 uint8* 指针实际对应就是地址
		// 2 &FuncTwoParams 结构体的地址，也转成(uint8*)
		// 3.1 首先呢，函数返回值的相对地址。并且函数参数（包括返回值）在内存中以连续的方式存储！！！
		//         ReturnValueOffset属性指定了返回值，是相对于参数列表的偏移量，以字节为单位。
		// 3.2 步骤2的 函数参数的地址 + UFunction返回值的地址偏移 = 返回值的正确地址
		uint8* RetValPtr = (uint8*) &FuncTwoParams + FunctionFour->ReturnValueOffset;
		// 地址强转
		int32* RetVal = (int32*)RetValPtr;
		
		//FWHelper::Debug(FString("-  Return value ---> ") + FString::FromInt(*RetVal), 10.f);
		//FWHelper::Debug(FString("-  count True ---> ") + FString::FromInt(FuncTwoParams.Count), 10.f);
	}
	
}

void ADecActor::ReflectInstance()
{
	// // URefObj 这个是 UObject名，或者路径都行
	UClass* RefObjClass = StaticLoadClass(UObject::StaticClass(), NULL, TEXT("URefObj"));
	// 转成UObject
	URefObj* RefObjIns = Cast<URefObj>(RefObjClass);
	// // RefObjIns->EchoInfo();
	//
	// UObject* RefObjPtr = StaticLoadObject(UObject::StaticClass(), NULL, TEXT("URefObj"));
	// // 转成UObject
	// URefObj* RefObjIns_2 = Cast<URefObj>(RefObjPtr);
	// // RefObjIns_2->EchoInfo();
	//
	// if (RefObjIns == RefObjIns_2)
	// {
	// 	// FWHelper::Debug(FString("-  两个指针实际是指向同一块内存，也就现在还没进行实例化，只是内存资源 "), 10.f);
	// }

	// FindObject只能查找那些已经在UPackage加载或已经序列化的对象。而LoadObject可以从资源包中加载任何UObject对象，而不需要进行UPackage加载或序列化。在执行FindObject时，应该确保所需要的UObject对象已经加载到内存中。而在执行LoadObject时，应该确保要加载的对象是存在于指定的资源包中。
	// 获取Enum
	// ANY_PACKAGE 表示资源包 FindObject<UClass>(要搜索的区域,类名);
	// 通过UE4的FindObject寻找所有ANY_PACKAGE反射到的Name，寻找到后实例化后返回
	UEnum* EnumPtr = FindObject<UEnum>((UObject*)ANY_PACKAGE, *FString("ERefState"), true);
	if (EnumPtr)
	{
		// FWHelper::Debug(EnumPtr->GetNameByIndex(1).ToString(), 10.f);
	}

	// LoadObject 把conent资源加载到 内存
	// ------Uec++生成Spawn
	// LoadObject<T>(直接用路径  )用来加载非蓝图资源，比如动画、贴图、音效等资源；
	// LoadClass<T>(无脑加 _C  )用来加载蓝图并获取蓝图Class，比如角色蓝图。如果要用蓝图创建对象，必须先通过LoadClass获取class，然后再通过SpawnActor生成对象。
	UBlueprint* RefActorObject = LoadObject<UBlueprint>(NULL, TEXT("Blueprint'/Game/Blueprint/Reflect/RefActorBP.RefActorBP'"));
	// 转成class
	TSubclassOf<AActor> RefActorClass = (UClass*)RefActorObject->GeneratedClass;
	// class 实例化
	GetWorld()->SpawnActor<AActor>(RefActorClass, FVector::ZeroVector + FVector(.0f), FRotator::ZeroRotator);


}

void ADecActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

