// Fill out your copyright notice in the Description page of Project Settings.


#include "DDCore/DDDriver.h"

#include "DDObject/DDOO.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADDDriver::ADDDriver()
{
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	Center = CreateDefaultSubobject<UDDCenterModule>(TEXT("Center"));
	Center->SetupAttachment(RootComponent);
}

void ADDDriver::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// 注册世界 和 Driver 驱动器 到 UDDCommon单例
	UDDCommon::Get()->InitDriver(this);

	// 在游戏运行之前，必须进行一次模组ID的设定，这里面会注册子模组到数组
	Center->IterChangeModuleType(Center, ModuleType);

	Center->TotalGatherModule(ModuleType);
	
	// 创建所有模组的模块
	Center->IterCreateManager(Center);
}

void ADDDriver::BeginPlay()
{
	Super::BeginPlay();
	
	RegisterGamePlay();
	
	// 迭代调用Init函数
	Center->IterModuleInit(Center);

}

void ADDDriver::RegisterGamePlay()
{
	// 获取GameInstance
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	// 如果GameInstance存在，并且继承自IIDDOO，那就手动注册进Center
	if (GameInstance && Cast<IDDOO>(GameInstance))
	{
		Cast<IDDOO>(GameInstance)->RegisterToModule("Center", "GameInstance", "GameInstance");
	}

	// 获取Controller并注册到全局公共类DDCommon
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController)
	{
		//DDH::Debug(10) << "No PlayerController  " << DDH::Endl();
	} else
	{
		UDDCommon::Get()->InitController(PlayerController);
	}
}

void ADDDriver::ExecuteFunction(DDModuleAgreement Agreement, DDParam* Param)
{
	Center->AllotExecuteFunction(Agreement, Param);
}

void ADDDriver::ExecuteFunction(DDObjectAgreement Agreement, DDParam* Param)
{
	Center->AllotExecuteFunction(Agreement, Param);
}

void ADDDriver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!IsBeginePlay)
	{
		// 迭代调用Begin函数
		Center->IterModuleBeginPlay(Center);
		// 只执行第一帧
		IsBeginePlay = true;
	} else
	{
		// 迭代调用Tick
		Center->IterModuleTick(Center, DeltaTime);
	}
}

bool ADDDriver::RegisterToModule(IDDOO* ObjectInst)
{
	// 调用中央模组进行注册
	return Center->RegisterToModule(ObjectInst);
}

#if WITH_EDITOR
void ADDDriver::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// 参数1 是否修改变量
	// 参数2 修改变量的名字  member ID 【成员ID】
	if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(ADDDriver, ModuleType))
	{
		Center->IterChangeModuleType(Center, ModuleType);
	}
}
#endif

