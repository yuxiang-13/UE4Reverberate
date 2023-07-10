// Fill out your copyright notice in the Description page of Project Settings.


#include "Wealth/AsyncClassActor.h"

#include "Engine/StreamableManager.h"

// Sets default values
AAsyncClassActor::AAsyncClassActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAsyncClassActor::BeginPlay()
{
	Super::BeginPlay();

	// 获取所有资源路径
	TArray<FSoftObjectPath> ObjectWealthPaths;
	for (int i = 0; i < ClassWealthPaths.Num(); ++i)
	{
		ObjectWealthPaths.Push(ClassWealthPaths[i].ToSoftObjectPath());
	}

	// 进行异步加载
	
	// 创建资源加载管理器
	WealthLoader = new FStreamableManager();
	// 执行异步加载，添加资源链接数组 和 加载完成回调
	WealthHandle = WealthLoader->RequestAsyncLoad(
	  ObjectWealthPaths,
		 FStreamableDelegate::CreateUObject(
			this,
			&AAsyncClassActor::LoadWealthComplated
		 )
	);
	
}

void AAsyncClassActor::LoadWealthComplated()
{
	// 加载完成回调，通过句柄，获取对应class
	TArray<UObject*> WealthObjects;
	
	WealthHandle->GetLoadedAssets(WealthObjects);
	for (int i = 0; i < WealthObjects.Num(); i++)
	{
		UClass* WealthClass = Cast<UClass>(WealthObjects[i]);
		//生成对象 , 注意, 如果UClass不是AActor的类型,
		//1 是UObject类型, 可以用NewObject生成实例,
		//2 如果是UUserWidget类型, 可以用CreateWidget生成实例
		
		/*
			UObject* InstObject = NewObject<UObject>(this, class);
			AActor* InstActor = GetWorld()->SpawnActor<AActor>(class, GetActorTransform());
			UUserWidget* InstWidget = CreateWidget<UUserWidget>(GetWorld(), class);
		 */
		
		AActor* WealthActor = GetWorld()->SpawnActor<AActor>(WealthClass, FVector(0.f), FQuat::Identity.Rotator());
		WealthActors.Push(WealthActor);
	}

	// 设置显示第一个
	IndexCounter = 0;
}

// Called every frame
void AAsyncClassActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (WealthHandle->HasLoadCompleted() && WealthActors.Num() > 0)
	{
		if (TimeCounter > 2.f)
		{
			TimeCounter = 0;

			//显示下一个Actor
			WealthActors[IndexCounter]->SetActorLocation(FVector(0.f, 0.f, 1000.f));
			IndexCounter = IndexCounter + 1 >= WealthActors.Num() ? 0 : IndexCounter + 1;
			WealthActors[IndexCounter]->SetActorLocation(GetActorLocation());
			
		} else {
			TimeCounter += DeltaTime;
		}
	}
}

