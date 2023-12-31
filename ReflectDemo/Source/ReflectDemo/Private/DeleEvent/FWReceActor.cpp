// Fill out your copyright notice in the Description page of Project Settings.


#include "DeleEvent/FWReceActor.h"

#include "Common/FWCommon.h"
#include "DeleEvent/FWDEActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFWReceActor::AFWReceActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFWReceActor::BeginPlay()
{
	Super::BeginPlay();

	// 获取A，并把函数传过去
	TArray<AActor*> ActArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFWDEActor::StaticClass(), ActArray);
	if (ActArray.Num() > 0)
	{
		AFWDEActor* A = Cast<AFWDEActor>(ActArray[0]);
		// 方法1： TFunction 的函数传递 (通过lambda传递)
		// auto plus = [] (int v1, int v2) -> int { return v1 + v2; }
		/*
		*   []：默认不捕获任何变量；
			[=]：默认以值捕获所有变量；
			[&]：默认以引用捕获所有变量；
			[x]：仅以值捕获x，其它变量不捕获；
			[&x]：仅以引用捕获x，其它变量不捕获；
			[=, &x]：默认以值捕获所有变量，但是x是例外，通过引用捕获；
			[&, x]：默认以引用捕获所有变量，但是x是例外，通过值捕获；
			[this]：通过引用捕获当前对象（其实是复制指针）；
			[*this]：通过传值方式捕获当前对象；
		 */
		// A->RegFuncOne(
		// 	[this](FString InfoStr)
		// 	{
		// 		EchoInfoOne(InfoStr);
		// 	}
		// );

		
		// 方法2： TMemFunPtrType 的函数传递
		// 1 this 类
		// 2 &AFWReceActor::EchoInfoTwo   &函数地址
		// A->RegFuncTwo(this, &AFWReceActor::EchoInfoTwo);



		
		// 方法3： FMethodPtr 的函数传递
		// A->RegFunThree(this, &AFWReceActor::EchoInfoTwo);


		/*
		//利用模板实现交换
		template<typename T>
		void mySwap(T& a, T& b)
		{
			// ...
		}
		  //1、自动类型推导
		  mySwap(a, b);
		  
		  //2、显示指定类型
		  mySwap<int>(a, b);


		template <typename T0, typename T1, typename T2, typename T3, typename T4>
		T2 func(T1 v1, T3 v2, T4 v3)
		{
			T0 static t0 = T0(0);
		
			T2 t2 = v1 + v2 + v3;
		
			return t2;
		};
		
		int main()
		{
			std::cout << func<double, int, int>(0, 1, 1.2) << std::endl;
		}

		1 其中T0和函数实参无关，所以必须指定，而T2和返回值相关，所以也必须指定，
		2 又因为模板的形参和实参是通过位置来进行关联的，所以T1夹在T0和T1之间，其也需要显式指定。
		3 3和T4则可以从实参列表中推导出来



		 */
		
		// 方法4： FMethodPtr 扩展
		// 定义委托
		DECLARE_DELEGATE_TwoParams(FTempDele, int32, FString)
		// RegFunFour<FTempDele> 的 <> ：为啥这个需要<>，因为涉及到了 模板根据形参自动类型推导。而且只用了类型，没传具体值
		// &AFWReceActor::EchoInfoTwo 这个对应 函数体第二个参数，也就是 这个FTemp  Delegate的类型绑定的 具体 方法 InMethod
		// A->RegFunFour<FTempDele>(this, &AFWReceActor::EchoInfoTwo, 100, FString("RegFunFour"));

		
		
		// //方法五  :  TFunction的扩展
		// TFunction<void(FString)> MyFunction_1 = [](FString Parameter) {
		// 	// 实现逻辑
		// 	FWHelper::Debug(FString("- - - >>> B MyFunction 函数 ") + Parameter, 10.f);
		// };
		// // 调用不定参数的泛型函数
		// // A->RegFunFive_HasB_Params<void>(MyFunction_1, FString("MyFunction_1"));

		
		// TFunction<bool(FString, int32)> MyFunction_2 = [this](FString InfoStr, int32 Count)
		// {
		// 	return EchoInfoThree(InfoStr, Count);
		// };
		// // A->RegFunFive_ASelf_Params<bool>(MyFunction_2);
	}
}

void AFWReceActor::EchoInfoOne(FString InfoStr)
{
	//FWHelper::Debug(FString("- - - >>> B 函数 ") + InfoStr, 10.f);
}

void AFWReceActor::EchoInfoTwo(int32 Count, FString InfoStr)
{
	//FWHelper::Debug(FString("- - - >>> B ") + InfoStr + FString("- -- -Count = ") + FString::FromInt(Count), 10.f);
}

bool AFWReceActor::EchoInfoThree(FString InfoStr, int32 Count)
{
	//FWHelper::Debug(InfoStr + FString(" --> ") + FString::FromInt(Count), 500.f);
	return true;
}



// Called every frame
void AFWReceActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

