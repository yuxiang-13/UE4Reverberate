// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameFramework/Actor.h"
#include "TopWealthActor.generated.h"




class UTexture2D;
class UObjectLibrary;

// 结构体类型
struct FStreamableManager;
struct FStreamableHandle;

// 步骤1扩展知识但 也可以创建 结构体类型
USTRUCT()
struct FWealthNode
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FName WealthName;

	UPROPERTY(EditAnywhere)
	FSoftObjectPath WealthPath;
};

// // 步骤1 声明DataAssest类
// UCLASS()
// class REFLECTDEMO_API UWealthData : public UDataAsset
// {
// 	GENERATED_BODY()
// public:
// 	UPROPERTY(EditAnywhere)
// 	TArray<UTexture2D*> WealthTexure;
//
// 	UPROPERTY(EditAnywhere)
// 	TArray<FWealthNode> WealthGroup;
// };


UCLASS()
class REFLECTDEMO_API ATopWealthActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATopWealthActor();

	void AssignWealthWidget(class UWealthWidget* InWidget);

	// 循环更新贴图
	void UpdateTexture();

	// 三种对象的关系 与 转换
	void UObjectUClassUBleprint();
public:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* WorkMesh;
	//
	// // 步骤2 使用DataAssest类
	// UPROPERTY(EditAnywhere)
	// UWealthData* WealthData;


	// 软引用三个 蓝图实例路径（只是路径）
	UPROPERTY(EditAnywhere)
	FSoftObjectPath HandleObjectPath;

	UPROPERTY(EditAnywhere)
	FSoftObjectPath HandleActorPath;
	
	UPROPERTY(EditAnywhere)
	FSoftObjectPath HandleWidgetPath;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 资源状态，FindObject 和 LoadObject
	void WealthState();
	
	void UpdateMesh();
	
	// 步骤3 声明函数
	void ObjectLibraryOperate();

	// 资源异步加载
	void StreamableManagerOperate();

	// 资源异步加载完成
	void StreamableManagerLoadComplete();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
protected:
	int32 MeshIndex;

	FTimerHandle UpdateMeshHandle;

	// 步骤1 声明UObjectLibrary
	/*
	1 ObjectLibrary：是一个对象，包含了一系列继承共享基类的未加载对象或者未加载对象的FAssetData 。
	您可以通过提供一个搜索路径来加载一个对象库，它将加载那个路径中的所有资源。
	2 我们只需要给它设定一个路径，然后让它来扫描，那么就不用我们手动来一个一添加了，会为我们省很多事。
	3 最好不要让ObjectLibrary给GC回收掉，所以可以添加到根节点，因为你不想还在扫描或者加载的途中出什么岔子。
	 */
	UObjectLibrary* ObjectLibrary;
	// 步骤2 声明 用于存储UObjectLibrary找到的资源
	TArray<FSoftObjectPath> TexturePaths;

	
	class UWealthWidget* wealthWidget;
	FStreamableManager* WealthLoader;
	TSharedPtr<FStreamableHandle> WealthHandle;
	
	TArray<UTexture2D*> TextureGroup;
	int32 TextureIndex;

	FTimerHandle UpdateTextureHandle;
};
