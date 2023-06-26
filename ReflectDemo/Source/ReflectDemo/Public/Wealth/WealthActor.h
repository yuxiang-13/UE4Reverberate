// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameFramework/Actor.h"
#include "WealthActor.generated.h"




class UTexture2D;
class UObjectLibrary;

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

// 步骤1 声明DataAssest类
UCLASS()
class REFLECTDEMO_API UWealthData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TArray<UTexture2D*> WealthTexure;

	UPROPERTY(EditAnywhere)
	TArray<FWealthNode> WealthGroup;
};


UCLASS()
class REFLECTDEMO_API AWealthActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWealthActor();

public:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* WorkMesh;
	
	// 步骤2 使用DataAssest类
	UPROPERTY(EditAnywhere)
	UWealthData* WealthData;
	
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 资源状态，FindObject 和 LoadObject
	void WealthState();

	
	void UpdateMesh();
	
	// 步骤3 声明函数
	void ObjectLibraryOperate();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
protected:
	int32 MeshIndex;

	FTimerHandle UpdateMeshHandle;

	// 步骤1 声明UObjectLibrary
	UObjectLibrary* ObjectLibrary;
	// 步骤2 声明 用于存储UObjectLibrary找到的资源
	TArray<FSoftObjectPath> TexturePaths;
};
